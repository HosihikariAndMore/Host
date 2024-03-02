#ifndef HOSTING_MAIN_H
#define HOSTING_MAIN_H

#include <nethost/coreclr_delegates.h>
#include <nethost/hostfxr.h>
#include <nethost/nethost.h>

typedef int (*__libc_start_main_type)(int (*)(int, char **, char **), int,
                                      char **, void (*)(), void (*)(),
                                      void (*)(), void *);

int __libc_start_main(int (*)(int, char **, char **), int, char **, void (*)(),
                      void (*)(), void (*)(), void *);

void init_loading();

#define LIBRARY_DIR_PATH "lib/"
#define MAIN_NAMESPACE "Hosihikari"
#define PLUGIN_MANAGEMENT_NAME "PluginManager"
#define PLUGIN_MANAGEMENT_NAMESPACE "PluginManagement"

#endif  // HOSTING_MAIN_H
