#ifndef AVT_PLUGIN_MODULE_LOGGER_H
#define AVT_PLUGIN_MODULE_LOGGER_H

#include "PluginModule_global.h"

namespace AVerMedia
{

class AVT_PLUGIN_MODULE_EXPORT Logger
{
public:
    virtual ~Logger() = default;
    virtual void log(const char* message) = 0;
};

}

#endif // AVT_PLUGIN_MODULE_LOGGER_H
