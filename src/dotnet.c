#include <assert.h>
#include <dlfcn.h>
#include <dotnet.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int load_hostfxr() {
    char_t buffer[PATH_MAX];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    int rc = get_hostfxr_path(buffer, &buffer_size, NULL);
    if (rc != 0) {
        return 0;
    }

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

int init_delegate_fptrs(const char_t *config_path) {
    hostfxr_handle cxt = NULL;
    int rc = init_fptr(config_path, NULL, &cxt);
    if (rc != 0 || cxt == NULL) {
        printf("Init failed: %x\n", rc);
        close_fptr(cxt);
        return 0;
    }

    rc =
        get_delegate_fptr(cxt, hdt_load_assembly_and_get_function_pointer,
                          (void **)load_assembly_and_get_function_pointer_fptr);
    if (rc != 0 || load_assembly_and_get_function_pointer_fptr == NULL) {
        printf(
            "Get delegate load_assembly_and_get_function_pointer failed: %x\n",
            rc);
        close_fptr(cxt);
        return 0;
    }

    //    rc = get_delegate_fptr(cxt, hdt_get_function_pointer,
    //                           (void **)get_function_pointer_fptr);
    //    if (rc != 0 || get_function_pointer_fptr == NULL) {
    //        printf("Get delegate get_function_pointer failed: %x\n", rc);
    //        close_fptr(cxt);
    //        return 0;
    //    }

    rc = get_delegate_fptr(cxt, hdt_load_assembly, (void **)load_assembly_fptr);
    if (rc != 0 || load_assembly_fptr == NULL) {
        printf("Get delegate load_assembly failed: %x\n", rc);
        close_fptr(cxt);
        return 0;
    }

    //    rc = get_delegate_fptr(cxt, hdt_load_assembly_bytes,
    //                           (void **)load_assembly_bytes_fptr);
    //    if (rc != 0 || load_assembly_bytes_fptr == NULL) {
    //        printf("Get delegate load_assembly_bytes failed: %x\n", rc);
    //        close_fptr(cxt);
    //        return 0;
    //    }

    close_fptr(cxt);
    return (
        load_assembly_and_get_function_pointer_fptr &&
        //            get_function_pointer_fptr && load_assembly_bytes_fptr &&
        load_assembly_fptr);
}