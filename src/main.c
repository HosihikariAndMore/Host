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
    char *resolved = realpath(LIBRART_DIR_PATH, lib_path);
    assert(resolved != NULL);
    int rc = load_assembly_fptr(
        strcat(resolved, "/" MAIN_NAMESPACE "." PLUGIN_MANAGER_NAME ".dll"),
        NULL, NULL);
    if (rc != 0) {
        printf("Load assembly failed: %x\n", rc);
    }
    assert(rc == 0 && "Failure: load_assembly()");

    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = NULL;
    rc = load_assembly_and_get_function_pointer_fptr(
        LIBRART_DIR_PATH MAIN_NAMESPACE "." ASSEMBLY_LOADER_NAME ".dll",
        MAIN_NAMESPACE "." ASSEMBLY_LOADER_NAME ".Main, " MAIN_NAMESPACE
                       "." ASSEMBLY_LOADER_NAME,
        "Initialize", UNMANAGEDCALLERSONLY_METHOD, NULL, (void **)&entry_point);
    if (rc != 0) {
        printf("Load assembly and get function pointer failed: %x\n", rc);
    }
    assert(rc == 0 && entry_point != NULL &&
           "Failure: load_assembly_and_get_function_pointer()");

    entry_point();
}
