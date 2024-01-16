#include <dotnet.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

void init_loading(ll::Logger& logger) {
    int rc = load_hostfxr();
    if (rc != 0) {
        logger.info("Failed to load hostfxr: 0x{:x}\n", rc);
        return;
    }
    rc = init_delegate_fptrs(LIBRARY_DIR_PATH MAIN_NAMESPACE
                             L"." PLUGIN_MANAGEMENT_NAME L".runtimeconfig.json");
    if (rc != 0) {
        logger.info("Failed to get functions from hostfxr: 0x{:x}\n", rc);
        return;
    }
    char_t plugin_management_path[MAX_PATH];
    DWORD size = ::GetFullPathNameW(LIBRARY_DIR_PATH, sizeof(plugin_management_path) / sizeof(char_t), plugin_management_path, nullptr);
    if (size == 0) {
        logger.info("Failed to get the path of the plugin management library: 0x{:x}\n", rc);
        return;
    }
    strcat(strcat((char *)plugin_management_path, "\\"),
           MAIN_NAMESPACE "." PLUGIN_MANAGEMENT_NAME ".dll");
    rc = load_assembly_fptr(plugin_management_path, NULL, NULL);
    if (rc != 0) {
        logger.info("Failed to load plugin management: 0x{:x}\n", rc);
        return;
    }
    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = NULL;
    rc = get_function_pointer_fptr(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main, " MAIN_NAMESPACE
                       L"." PLUGIN_MANAGEMENT_NAME,
        L"Initialize", UNMANAGEDCALLERSONLY_METHOD, NULL, NULL,
        (void **)&entry_point);
    if (rc != 0) {
        logger.info("Failed to get entry point from plugin management: 0x{:x}\n", rc);
        return;
    }
    entry_point();
}
