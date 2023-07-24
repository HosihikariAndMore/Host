#include <assert.h>
#include <dlfcn.h>
#include <dotnet.h>

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

    const char_t *config_path = "lib/AssemblyLoader.runtimeconfig.json";
    if (init_delegate_fptrs(config_path)) {
        assert(0 && "Failure: init_delegate_fptrs()");
    }

    const char_t *dotnetlib_path = "lib/AssemblyLoader.dll";
    const char_t *dotnet_type =
        "Hosihikari.AssemblyLoader.Main, AssemblyLoader";
    const char_t *dotnet_type_method = "Initialize";
    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = NULL;
    int rc = load_assembly_and_get_function_pointer_fptr(
        dotnetlib_path, dotnet_type, dotnet_type_method,
        UNMANAGEDCALLERSONLY_METHOD, NULL, (void **)&entry_point);
    assert(rc == 0 && entry_point != NULL &&
           "Failure: load_assembly_and_get_function_pointer()");
    load_assembly_fptr("lib/PluginManager.dll", NULL, NULL);

    entry_point();
}
