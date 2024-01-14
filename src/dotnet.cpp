#include <dotnet.h>
#include <Windows.h>

load_assembly_fn load_assembly_fptr;
get_function_pointer_fn get_function_pointer_fptr;

hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;

int load_hostfxr() {
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    int rc = get_hostfxr_path(buffer, &buffer_size, NULL);
    if (rc != 0) {
        return rc;
    }
    HMODULE lib = ::LoadLibraryW(buffer);
    if (lib == NULL) {
        return -1;
    }
    init_fptr = (hostfxr_initialize_for_runtime_config_fn)::GetProcAddress(
        lib, "hostfxr_initialize_for_runtime_config");
    if (init_fptr == NULL) {
        ::FreeLibrary(lib);
        return -1;
    }
    get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)::GetProcAddress(
        lib, "hostfxr_get_runtime_delegate");
    if (get_delegate_fptr == NULL) {
        ::FreeLibrary(lib);
        return -1;
    }
    close_fptr = (hostfxr_close_fn)::GetProcAddress(lib, "hostfxr_close");
    if (close_fptr == NULL) {
        ::FreeLibrary(lib);
        return -1;
    }
    return 0;
}

int init_delegate_fptrs(const char_t *config_path) {
    hostfxr_handle cxt = NULL;
    int rc = init_fptr(config_path, NULL, &cxt);
    if (rc != 0 || cxt == NULL) {
        close_fptr(cxt);
        return rc;
    }
    rc =
        get_delegate_fptr(cxt, hdt_load_assembly, (void **)&load_assembly_fptr);
    if (rc != 0 || load_assembly_fptr == NULL) {
        close_fptr(cxt);
        return rc;
    }
    rc = get_delegate_fptr(cxt, hdt_get_function_pointer,
                           (void **)&get_function_pointer_fptr);
    if (rc != 0 || get_function_pointer_fptr == NULL) {
        close_fptr(cxt);
        return rc;
    }
    close_fptr(cxt);
    return 0;
}