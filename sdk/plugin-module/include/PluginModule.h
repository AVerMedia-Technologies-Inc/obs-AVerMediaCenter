#ifndef AVT_PLUGIN_MODULE_H
#define AVT_PLUGIN_MODULE_H

#include "PluginModule_global.h"

#include <memory>
#include <functional>
#include <string>

class QMainWindow;

namespace AVerMedia
{

class Logger;
class AVT_PLUGIN_MODULE_EXPORT PluginModule
{
    struct Impl;
    std::unique_ptr<Impl> m_impl;

public:
    /**
     * @brief Construct a new Plugin Module object
     *
     * @param language language code, e.g. en_US
     * @param logger logger object
     * @param version OBS current core version string
     * @param build OBS current core version
     */
    PluginModule(const char* language = nullptr, Logger* logger = nullptr,
                 const char* version = nullptr, int build = 0);
    ~PluginModule();

    /**
     * @brief Load the plugin module. Usually called in `obs_module_load(void)`
     *
     * @param mainWindow Qt main window. We can get it from `obs_frontend_get_main_window()`.
     */
    void load(QMainWindow* mainWindow);

    /**
     * @brief Post-load the plugin module. Usually called in `obs_module_post_load(void)`
     */
    void postLoad();

    /**
     * @brief Send OBS frontend event to plugin module.
     *
     * @param what event type
     * @param data event data
     */
    void frontendEvent(int what, const char* data = nullptr);

    /**
     * @brief Set plugin module backend event handler.
     * @param handler event handler
     */
    void setBackendEventHandler(std::function<void(std::string, int, const char*)> handler);

    /**
     * @brief Send event to plugin module backend.
     *
     * @param model event model
     * @param what event type
     * @param data event data
     */
    void sendEvent(std::string model, int what, const char* data = nullptr);

    /**
     * @brief Unload the plugin module. Usually called in `obs_module_unload(void)`
     */
    void unload();
};

}

#endif // AVT_PLUGIN_MODULE_H
