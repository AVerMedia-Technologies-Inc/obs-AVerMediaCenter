#ifndef BROSWERINTERFACE_HPP
#define BROSWERINTERFACE_HPP

#include <string>

typedef void (*SetStartupScriptFunc)(const std::string& script, void* widget);
typedef void (*ExecuteJavaScriptFunc)(const std::string& script, void* widget);

#endif // BROSWERINTERFACE_HPP
