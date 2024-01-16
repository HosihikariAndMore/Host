#ifndef PRELOADER_MAIN_H
#define PRELOADER_MAIN_H

#include <nethost/coreclr_delegates.h>
#include <nethost/hostfxr.h>
#include <nethost/nethost.h>
#include <ll/api/logger.h>

void init_loading(ll::Logger&);

#define LIBRARY_DIR_PATH L".\\plugins\\" MAIN_NAMESPACE ".Preload\\"
#define MAIN_NAMESPACE "Hosihikari"
#define PLUGIN_MANAGEMENT_NAME "PluginManager"
#define PLUGIN_MANAGEMENT_NAMESPACE L"PluginManagement"

#endif  // PRELOADER_MAIN_H
