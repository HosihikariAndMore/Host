#include <dlfcn.h>
#include <dotnet.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int __libc_start_main(int (*main)(int, char **, char **), int argc,
                      char **ubp_av, void (*init)(), void (*fini)(),
                      void (*rtld_fini)(), void *stack_end) {
    init_loading();
    __libc_start_main_type original_start_main =
        (__libc_start_main_type)dlsym((void *)-1, "__libc_start_main");
    return original_start_main(main, argc, ubp_av, init, fini, rtld_fini,
                               stack_end);
}

void init_loading() {
    int rc = load_hostfxr();
    if (rc != 0) {
        printf("Failed to load hostfxr: 0x%x\n", rc);
        return;
    }
    rc = init_delegate_fptrs(LIBRARY_DIR_PATH MAIN_NAMESPACE
                             "." PLUGIN_MANAGEMENT_NAME ".runtimeconfig.json");
    if (rc != 0) {
        printf("Failed to get functions from hostfxr: 0x%x\n", rc);
        return;
    }
    char_t plugin_management_path[PATH_MAX];
    strcat(strcat(realpath(LIBRARY_DIR_PATH, plugin_management_path), "/"),
           MAIN_NAMESPACE "." PLUGIN_MANAGEMENT_NAME ".dll");
    rc = load_assembly_fptr(plugin_management_path, NULL, NULL);
    if (rc != 0) {
        printf("Failed to load plugin management: 0x%x\n", rc);
        return;
    }
    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = NULL;
    rc = get_function_pointer_fptr(
        MAIN_NAMESPACE "." PLUGIN_MANAGEMENT_NAMESPACE ".Main, " MAIN_NAMESPACE
                       "." PLUGIN_MANAGEMENT_NAME,
        "Initialize", UNMANAGEDCALLERSONLY_METHOD, NULL, NULL,
        (void **)&entry_point);
    if (rc != 0) {
        printf("Failed to get entry point from plugin management: 0x%x\n", rc);
        return;
    }
    entry_point();
}
