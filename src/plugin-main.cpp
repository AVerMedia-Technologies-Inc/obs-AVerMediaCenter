/*
AVerMedia Assets
Copyright (C) 2024 AVerMedia Technologies Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <plugin-support.h>

#include <QMainWindow>
#include <QCoreApplication>

#include "PluginModule.h"
#include "Logger.h"
#include "AvtEvent.hpp"
#include "ObsCloseHelper.h"

#ifdef ENABLE_OBS_BROWSER_INTERFACE
#include "obs-browser-panel.hpp"
#include "BrowserInterface.hpp"
#endif // ENABLE_OBS_BROWSER_INTERFACE

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

#ifdef ENABLE_OBS_BROWSER_INTERFACE
static QCef* cef = nullptr;
//static QCefCookieManager* panel_cookies = nullptr;
static bool cef_js_avail = false;

static void setStartupScriptImpl(const std::string& script, void* widget) {
	if (QCefWidget* cefWidget = static_cast<QCefWidget*>(widget)) {
		cefWidget->setStartupScript(script);
	}
}

static void executeJavaScriptImpl(const std::string& script, void* widget) {
	if (QCefWidget* cefWidget = static_cast<QCefWidget*>(widget)) {
		cefWidget->executeJavaScript(script);
	}
}

#endif // ENABLE_OBS_BROWSER_INTERFACE

// implement obs logging
class AvtLogger : public AVerMedia::Logger
{
	void log(const char* message) override
	{
		obs_log(LOG_INFO, "%s", message);
	}
};

/*static*/ AvtLogger* logger = nullptr;
/*static*/ AVerMedia::PluginModule* module = nullptr;

static void obs_frontend_event_handler(enum obs_frontend_event event, void *)
{
	switch (event) {
	case OBS_FRONTEND_EVENT_EXIT:
		//obs_log(LOG_INFO, "OBS_FRONTEND_EVENT_EXIT");
		if (module) module->frontendEvent(1000 + event);
		break;
	case OBS_FRONTEND_EVENT_FINISHED_LOADING:
		//obs_log(LOG_INFO, "OBS_FRONTEND_EVENT_FINISHED_LOADING");
		if (module) module->frontendEvent(1000 + event);
		break;
	case OBS_FRONTEND_EVENT_THEME_CHANGED: // added @ OBS 29.0.0
		//obs_log(LOG_INFO, "OBS_FRONTEND_EVENT_THEME_CHANGED");
		if (module) module->frontendEvent(1000 + event);
		break;
	default:
		//obs_log(LOG_INFO, "obs_frontend_event_handler: %d", event);
		if (module) module->frontendEvent(1000 + event);
		break;
	}
}

#define PRINT_OBS_LOG(data, level, prefix) \
do { \
	auto json = obs_data_create_from_json(data); \
	auto msg = obs_data_get_string(json, "message"); \
	obs_log(LOG_INFO, "%s: %s", prefix, msg); \
	obs_data_release(json); \
} while(0);

static void avt_backend_event_handler(std::string model, int what, const char* data)
{
	switch (what) {
	case AVerMedia::EventType::LOG_DEBUG:
		PRINT_OBS_LOG(data, LOG_DEBUG, "D");
		break;
	case AVerMedia::EventType::LOG_INFO:
		PRINT_OBS_LOG(data, LOG_INFO, "I");
		break;
	case AVerMedia::EventType::LOG_WARNING:
		PRINT_OBS_LOG(data, LOG_WARNING, "W");
		break;
	case AVerMedia::EventType::LOG_ERROR:
		PRINT_OBS_LOG(data, LOG_ERROR, "E");
		break;

	case AVerMedia::EventType::AVT_EVENT_START_STREAMING:
		obs_frontend_streaming_start();
		break;
	case AVerMedia::EventType::AVT_EVENT_STOP_STREAMING:
		obs_frontend_streaming_stop();
		break;
	case AVerMedia::EventType::AVT_EVENT_START_RECORDING:
		obs_frontend_recording_start();
		break;
	case AVerMedia::EventType::AVT_EVENT_STOP_RECORDING:
		obs_frontend_recording_stop();
		break;
	case AVerMedia::EventType::AVT_EVENT_TAKE_SCREENSHOT:
		obs_frontend_take_screenshot();
		break;

	case AVerMedia::EventType::AVT_EVENT_QUERY_STREAMING:
	{
		auto active = obs_frontend_streaming_active();
		auto json = obs_data_create();
		obs_data_set_bool(json, "value", active);
		auto str = obs_data_get_json(json);
		if (module) module->sendEvent(model, AVerMedia::EventType::AVT_EVENT_QUERY_STREAMING, str);
		obs_data_release(json);
	}
		break;
	case AVerMedia::EventType::AVT_EVENT_QUERY_RECORDING:
	{
		auto active = obs_frontend_recording_active();
		auto json = obs_data_create();
		obs_data_set_bool(json, "value", active);
		auto str = obs_data_get_json(json);
		if (module) module->sendEvent(model, AVerMedia::EventType::AVT_EVENT_QUERY_RECORDING, str);
		obs_data_release(json);
	}
		break;
	case AVerMedia::EventType::AVT_EVENT_QUERY_THEME:
	{
		auto isDark = obs_frontend_is_theme_dark();
		auto json = obs_data_create();
		obs_data_set_bool(json, "value", isDark);
		auto str = obs_data_get_json(json);
		if (module) module->sendEvent(model, AVerMedia::EventType::AVT_EVENT_QUERY_THEME, str);
		obs_data_release(json);
	}
		break;
	case AVerMedia::EventType::AVT_EVENT_QUERY_LAST_RECORDING:
	{
		auto path = obs_frontend_get_last_recording();
		auto json = obs_data_create();
		obs_data_set_string(json, "value", path);
		bfree(path);
		auto str = obs_data_get_json(json);
		if (module) module->sendEvent(model, AVerMedia::EventType::AVT_EVENT_QUERY_LAST_RECORDING, str);
		obs_data_release(json);
	}
		break;
	case AVerMedia::EventType::AVT_EVENT_QUERY_LAST_SCREENSHOT:
	{
		auto path = obs_frontend_get_last_screenshot();
		auto json = obs_data_create();
		obs_data_set_string(json, "value", path);
		bfree(path);
		auto str = obs_data_get_json(json);
		if (module) module->sendEvent(model, AVerMedia::EventType::AVT_EVENT_QUERY_LAST_SCREENSHOT, str);
		obs_data_release(json);
	}
		break;

	case AVerMedia::EventType::AVT_EVENT_SHUTDOWN:
	{
		obs_log(LOG_WARNING, "AVerMedia::EventType::AVT_EVENT_SHUTDOWN");
		//qApp->exit();
		QMainWindow *obsMain = (QMainWindow *)obs_frontend_get_main_window();
		ObsCloseHelper::close(obsMain);
		//QCoreApplication::postEvent(qApp, new QEvent(QEvent::Close));
	}
		break;

#ifdef ENABLE_OBS_BROWSER_INTERFACE
	case AVerMedia::EventType::AVT_EVENT_QUERY_BROWSER_INSTANCE:
	{
		if (cef == nullptr) {
			cef = obs_browser_init_panel();
			cef_js_avail = cef && obs_browser_qcef_version() >= 3;
			obs_log(LOG_INFO, "CEF initialized: %p %d", cef, cef_js_avail);
			if (cef) {
				cef->init_browser();
			}
		}
		if (cef == nullptr) {
			obs_log(LOG_ERROR, "ERROR CEF not initialized");
			return;
		}
		
		auto inputJson = obs_data_create_from_json(data);
		auto targetUrl = obs_data_get_string(inputJson, "url");		
		obs_log(LOG_INFO, "cef->create_widget ++");
		QCefWidget* cefWidget = cef->create_widget(nullptr, targetUrl);
		obs_log(LOG_INFO, "cef->create_widget --");
		QString cefWidgetPtrString = QString::number(reinterpret_cast<quintptr>(cefWidget), 16);
		QByteArray cefWidgetPtrStringBytes = cefWidgetPtrString.toUtf8();
		
		SetStartupScriptFunc setStartupScriptFunc = &setStartupScriptImpl;
		ExecuteJavaScriptFunc executeJavaScriptFunc = &executeJavaScriptImpl;

		QString setStartupScriptFuncString = QString::number(reinterpret_cast<quintptr>(setStartupScriptFunc), 16);
		QByteArray setStartupScriptFuncStringBytes = setStartupScriptFuncString.toUtf8();

		QString executeJavaScriptFuncString = QString::number(reinterpret_cast<quintptr>(executeJavaScriptFunc), 16);
		QByteArray executeJavaScriptFuncStringBytes = executeJavaScriptFuncString.toUtf8();

		auto json = obs_data_create();
		obs_data_apply(json, inputJson); // OPFSC-1181: Merges the data of inputJson in to json.
		obs_data_set_string(json, "cefWidgetPtr", cefWidgetPtrStringBytes.constData());
		obs_data_set_string(json, "setStartupScriptPtr", setStartupScriptFuncStringBytes.constData());
		obs_data_set_string(json, "executeJavaScriptPtr", executeJavaScriptFuncStringBytes.constData());

		auto str = obs_data_get_json(json);
		if (module) module->sendEvent(model, AVerMedia::EventType::AVT_EVENT_QUERY_BROWSER_INSTANCE, str);
		
		obs_data_release(inputJson);
		obs_data_release(json);
	}
	break;
#endif // ENABLE_OBS_BROWSER_INTERFACE

	default:
		obs_log(LOG_INFO, "%s %d: %s", model.c_str(), what, data);
		break;
	}
}

bool obs_module_load(void)
{
	//obs_log(LOG_INFO, "obs version %s (%d)", obs_get_version_string(), obs_get_version());
	obs_log(LOG_INFO, "plugin version %s (%s)", PLUGIN_VERSION, MY_BUILT_TIME_STR);

	obs_frontend_add_event_callback(obs_frontend_event_handler, nullptr);
	
	logger = new AvtLogger();

	auto locale = obs_module_text("AVT.STREAMING_CENTER.LOCALE");
	obs_log(LOG_INFO, "locale = %s", locale);

	QMainWindow *obsMain = (QMainWindow *)obs_frontend_get_main_window();
	auto vstring = obs_get_version_string();
	auto vcode = obs_get_version();
	
	module = new AVerMedia::PluginModule(locale, logger);
	module->setBackendEventHandler(avt_backend_event_handler);
	module->load(obsMain, vstring, vcode);

	return true;
}

void obs_module_post_load(void)
{
	//obs_log(LOG_INFO, "obs_module_post_load");
	if (module) module->postLoad();
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "obs_module_unload ENTER");
	
	obs_frontend_remove_event_callback(obs_frontend_event_handler, nullptr);

	if (module) {
		module->setBackendEventHandler(nullptr);
		module->unload();
		delete module;
		module = nullptr;
	}
	
	if (logger) {
		delete logger;
		logger = nullptr;
	}
	
	obs_log(LOG_INFO, "obs_module_unload LEAVE");
}
