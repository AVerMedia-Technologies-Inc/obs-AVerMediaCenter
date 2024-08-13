/**
 * In this file we re-implement the plugin module by using dynamic library.
 */
#include "PluginModule.h"

#include <QString>
#include <QMainWindow>
#include <QMenuBar>

#ifdef ENABLE_QT_TEST_CODE
#include <QCoreApplication>
#include <QDir>
#else
#include <obs-module.h>
//#include <util/platform.h>
#endif // ENABLE_QT_TEST_CODE
#include <dlfcn.h>

#include "Logger.h"

using namespace AVerMedia;

struct PluginModule::Impl
{
    void* module = nullptr;
    void* plugin = nullptr;
    Logger* logger = nullptr;
    void log(const char* msg)
    {
        if (logger) {
            logger->log(msg);
        }
    }
};

PluginModule::PluginModule(const char* locale, Logger* logger, const char* version, int build)
    : m_impl(std::make_unique<Impl>())
{
    m_impl->logger = logger;

    char dylibPath[1000];
#ifdef ENABLE_QT_TEST_CODE
    auto appDir = QDir(qApp->applicationDirPath());
    qDebug() << appDir.path();
    for (int i = 0; i < 1; i++) appDir.cdUp();
    //auto dylibFile = appDir.filePath("obs-plugin-core/sdk/plugin-module/lib/libAvtPluginModule.dylib");
    auto dylibFile = appDir.filePath("Resources/libAvtPluginModule.dylib");
    //sprintf(dylibPath, "/Users/jimmyhu/work/OBS_Plugin_Prototype/obs-plugin-core/sdk/plugin-module/bin/libAvtPluginModule.dylib");
    snprintf(dylibPath, 1000, "%s", dylibFile.toStdString().c_str());
    qDebug() << dylibPath;
#else
    /*m_impl->module =*/ obs_get_module_lib(obs_current_module());
    m_impl->log(locale);
    //auto binaryPath = obs_get_module_binary_path(obs_current_module());
    //m_impl->log(binaryPath);
    auto dataPath = obs_get_module_data_path(obs_current_module());
    //m_impl->log(dataPath);
    snprintf(dylibPath, 1000, "%s/libAvtPluginModule.dylib", dataPath);
    m_impl->log(dylibPath);
#endif // ENABLE_QT_TEST_CODE

    m_impl->module = dlopen(dylibPath, RTLD_GLOBAL);
    if (m_impl->module == nullptr) {
        m_impl->log("PluginModuleMac::dlopen() failed");
        return;
    }

    typedef void* (*CTOR_FUNC)(const char*, Logger*, const char*, int);
    auto ctor = (CTOR_FUNC)dlsym(m_impl->module, "createPluginModule");
    if (ctor) {
        //m_impl->log("PluginModuleMac::createPluginModule()");
        m_impl->plugin = ctor(locale, logger, version, build);
    }
}

PluginModule::~PluginModule()
{
    //delete m_impl->plugin;
}

void PluginModule::load(QMainWindow* mainWindow)
{
    //m_impl->log("PluginModuleMac::load()");
    typedef void (*LOAD_FUNC)(void*, QMainWindow*);
    auto loadFunc = (LOAD_FUNC)dlsym(m_impl->module, "loadPluginModule");
    if (loadFunc) {
        //m_impl->log("PluginModuleMac::loadPluginModule()");
        loadFunc(m_impl->plugin, mainWindow);
    }
}

void PluginModule::postLoad()
{
    //m_impl->log("PluginModuleMac::postLoad()");
    typedef void (*POST_LOAD_FUNC)(void*);
    auto postLoadFunc = (POST_LOAD_FUNC)dlsym(m_impl->module, "postLoadPluginModule");
    if (postLoadFunc) {
        //m_impl->log("PluginModuleMac::postLoadPluginModule()");
        postLoadFunc(m_impl->plugin);
    }
}

void PluginModule::frontendEvent(int what, const char* data)
{
    //m_impl->log("PluginModuleMac::frontendEvent()");
    typedef void (*FRONTEND_EVENT_FUNC)(void*, int, const char*);
    auto frontendEventFunc = (FRONTEND_EVENT_FUNC)dlsym(m_impl->module, "frontendEventPluginModule");
    if (frontendEventFunc) {
        //m_impl->log("PluginModuleMac::frontendEventPluginModule()");
        frontendEventFunc(m_impl->plugin, what, data);
    }
}

void PluginModule::setBackendEventHandler(std::function<void(std::string, int, const char*)> handler)
{
    //m_impl->log("PluginModuleMac::setBackendEventHandler()");
    typedef void (*BACKEND_EVENT_HANDLER_FUNC)(void*, std::function<void(std::string, int, const char*)>);
    auto backendEventHandlerFunc = (BACKEND_EVENT_HANDLER_FUNC)dlsym(m_impl->module,
                                                                     "backendEventHandlerPluginModule");
    if (backendEventHandlerFunc) {
        //m_impl->log("PluginModuleMac::backendEventHandlerPluginModule()");
        backendEventHandlerFunc(m_impl->plugin, handler);
    }
}

void PluginModule::sendEvent(std::string model, int what, const char* data)
{
    //m_impl->log("PluginModuleMac::sendEvent()");
    typedef void (*SEND_EVENT_FUNC)(void*, std::string, int, const char*);
    auto sendEventFunc = (SEND_EVENT_FUNC)dlsym(m_impl->module, "sendEventPluginModule");
    if (sendEventFunc) {
        //m_impl->log("PluginModuleMac::sendEventPluginModule()");
        sendEventFunc(m_impl->plugin, model, what, data);
    }
}

void PluginModule::unload()
{
    m_impl->log("PluginModuleMac::unload()");
    typedef void (*UNLOAD_FUNC)(void*);
    auto unloadFunc = (UNLOAD_FUNC)dlsym(m_impl->module, "unloadPluginModule");
    if (unloadFunc) {
        //m_impl->log("PluginModuleMac::unloadPluginModule()");
        unloadFunc(m_impl->plugin);
    }

    // MUST delete PluginModule instance here, moving to destructor will cause memory bad access
    if (m_impl->plugin) {
        typedef void (*DTOR_FUNC)(void*);
        auto dtor = (DTOR_FUNC)dlsym(m_impl->module, "destroyPluginModule");
        if (dtor) {
            //m_impl->log("PluginModuleMac::destroyPluginModule()");
            dtor(m_impl->plugin);
        }
        m_impl->plugin = nullptr;
    }
    if (m_impl->module) {
        dlclose(m_impl->module);
        m_impl->module = nullptr;
    }
    m_impl->log("PluginModuleMac::destroy()");
    m_impl->logger = nullptr;
}
