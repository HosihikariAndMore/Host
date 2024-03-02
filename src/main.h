#ifndef HOSTING_MAIN_H
#define HOSTING_MAIN_H

#include <ll/api/logger.h>
#include <nethost/coreclr_delegates.h>
#include <nethost/hostfxr.h>
#include <nethost/nethost.h>

void init_loading(ll::Logger&);

#define LIBRARY_DIR_PATH            L".\\plugins\\" MAIN_NAMESPACE "\\"
#define MAIN_NAMESPACE              "Hosihikari"
#define PLUGIN_MANAGEMENT_NAME      "PluginManager"
#define PLUGIN_MANAGEMENT_NAMESPACE L"PluginManagement"

#endif // HOSTING_MAIN_H
