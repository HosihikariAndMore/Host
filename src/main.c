#include <assert.h>
#include <dlfcn.h>
#include <dotnet.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int __libc_start_main(int (*main)(int, char **, char **), int argc,
                      char **ubp_av, void (*init)(), void (*fini)(),
                      void (*rtld_fini)(), void *stack_end) {
    init_loader();
    __libc_start_main_type original_start_main =
        (__libc_start_main_type)dlsym((void *)-1, "__libc_start_main");
    return original_start_main(main, argc, ubp_av, init, fini, rtld_fini,
                               stack_end);
}

void init_loader() {
    if (!load_hostfxr()) {
        assert(0 && "Failure: load_hostfxr()");
    }

    if (!init_delegate_fptrs(LIBRART_DIR_PATH MAIN_NAMESPACE
                             "." ASSEMBLY_LOADER_NAME ".runtimeconfig.json")) {
        assert(0 && "Failure: init_delegate_fptrs()");
    }

    char_t lib_path[PATH_MAX];
    char *resolved = strcat(realpath(LIBRART_DIR_PATH, lib_path), "/");
    assert(resolved != NULL);

    char_t plugin_manager_path[PATH_MAX];
    strcpy(plugin_manager_path, lib_path);
    strcat(plugin_manager_path, MAIN_NAMESPACE "." PLUGIN_MANAGER_NAME ".dll");
    int rc = load_assembly_fptr(plugin_manager_path, NULL, NULL);
    if (rc != 0) {
        printf("Load assembly plugin manager failed: %x\n", rc);
    }
    assert(rc == 0 && "Failure: load_assembly()");

    char_t assembly_loader_path[PATH_MAX];
    strcpy(assembly_loader_path, lib_path);
    strcat(assembly_loader_path,
           MAIN_NAMESPACE "." ASSEMBLY_LOADER_NAME ".dll");
    rc = load_assembly_fptr(assembly_loader_path, NULL, NULL);
    if (rc != 0) {
        printf("Load assembly assembly loader failed: %x\n", rc);
    }
    assert(rc == 0 && "Failure: load_assembly()");

    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = NULL;
    rc = get_function_pointer_fptr(MAIN_NAMESPACE "." ASSEMBLY_LOADER_NAME
                                                  ".Main, " MAIN_NAMESPACE
                                                  "." ASSEMBLY_LOADER_NAME,
                                   "Initialize", UNMANAGEDCALLERSONLY_METHOD,
                                   NULL, NULL, (void **)&entry_point);
    if (rc != 0) {
        printf("Get function pointer failed: %x\n", rc);
    }
    assert(rc == 0 && entry_point != NULL &&
           "Failure: get_function_pointer()");

    entry_point();
}
