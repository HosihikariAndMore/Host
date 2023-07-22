#ifndef PRELOADER_MAIN_H
#define PRELOADER_MAIN_H

#include <assert.h>
#include <dlfcn.h>
#include <limits.h>
#include <nethost/coreclr_delegates.h>
#include <nethost/hostfxr.h>
#include <nethost/nethost.h>
#include <stdio.h>

#define MANAGED_LOADER_NAME "Loader"

typedef int (*__libc_start_main_type)(int (*main)(int, char **, char **),
                                      int argc, char **ubp_av, void (*init)(),
                                      void (*fini)(), void (*rtld_fini)(),
                                      void *stack_end);

int __libc_start_main(int (*main)(int, char **, char **), int argc,
                      char **ubp_av, void (*init)(), void (*fini)(),
                      void (*rtld_fini)(), void *stack_end);

void init_loader();

hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;

int load_hostfxr();

load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(
    const char_t *assembly);

#endif  // PRELOADER_MAIN_H
