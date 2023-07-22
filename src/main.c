#include "main.h"

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

    const char_t *config_path =
        LIBRARY_DIR MANAGED_LOADER_NAME ".runtimeconfig.json";
    load_assembly_and_get_function_pointer_fn
        load_assembly_and_get_function_pointer =
            get_dotnet_load_assembly(config_path);
    assert(load_assembly_and_get_function_pointer != NULL &&
           "Failure: get_dotnet_load_assembly()");

    const char_t *dotnetlib_path = LIBRARY_DIR MANAGED_LOADER_NAME ".dll";
    const char_t *dotnet_type = "Loader.Main, Loader";
    const char_t *dotnet_type_method = "Initialize";
    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = NULL;
    int rc = load_assembly_and_get_function_pointer(
        dotnetlib_path, dotnet_type, dotnet_type_method,
        UNMANAGEDCALLERSONLY_METHOD, NULL, (void **)&entry_point);
    assert(rc == 0 && entry_point != NULL &&
           "Failure: load_assembly_and_get_function_pointer()");

    entry_point();
}

int load_hostfxr() {
    char_t buffer[PATH_MAX];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    int rc = get_hostfxr_path(buffer, &buffer_size, NULL);
    if (rc != 0) return 0;

    void *lib = dlopen(buffer, RTLD_LAZY | RTLD_LOCAL);
    assert(lib != NULL && "Failure: dlopen()");
    init_fptr = (hostfxr_initialize_for_runtime_config_fn)dlsym(
        lib, "hostfxr_initialize_for_runtime_config");
    assert(init_fptr != NULL &&
           "Failure: hostfxr_initialize_for_runtime_config()");
    get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)dlsym(
        lib, "hostfxr_get_runtime_delegate");
    assert(get_delegate_fptr != NULL &&
           "Failure: hostfxr_get_runtime_delegate()");
    close_fptr = (hostfxr_close_fn)dlsym(lib, "hostfxr_close");
    assert(close_fptr != NULL && "Failure: hostfxr_close()");

    return (init_fptr && get_delegate_fptr && close_fptr);
}

load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(
    const char_t *config_path) {
    void *load_assembly_and_get_function_pointer = NULL;
    hostfxr_handle cxt = NULL;
    int rc = init_fptr(config_path, NULL, &cxt);
    if (rc != 0 || cxt == NULL) {
        printf("Init failed: %x\n", rc);
        close_fptr(cxt);
        return NULL;
    }

    rc = get_delegate_fptr(cxt, hdt_load_assembly_and_get_function_pointer,
                           &load_assembly_and_get_function_pointer);
    if (rc != 0 || load_assembly_and_get_function_pointer == NULL)
        printf("Get delegate failed: %x\n", rc);

    close_fptr(cxt);
    return (load_assembly_and_get_function_pointer_fn)
        load_assembly_and_get_function_pointer;
}
