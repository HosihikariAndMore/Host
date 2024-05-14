#include <dotnet.h>
#include <filesystem>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_loading(ll::Logger& logger) {
    int rc = load_hostfxr();
    if (rc != 0) {
        logger.info("Failed to load hostfxr: {}\n", rc);
        return;
    }
    rc = init_delegate_fptrs(LIBRARY_DIR_PATH MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME L".runtimeconfig.json");
    if (rc != 0) {
        logger.info("Failed to get functions from hostfxr: {}\n", rc);
        return;
    }
    std::filesystem::path plugin_management_path =
        std::filesystem::absolute(LIBRARY_DIR_PATH MAIN_NAMESPACE "." PLUGIN_MANAGEMENT_NAME ".dll");
    rc = load_assembly_fptr(plugin_management_path.c_str(), nullptr, nullptr);
    if (rc != 0) {
        logger.info("Failed to load plugin management: {}\n", rc);
        return;
    }
    //typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    //entry_point_fn entry_point = nullptr;
    //rc                         = get_function_pointer_fptr(
    //    MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main, " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
    //    L"Initialize",
    //    UNMANAGEDCALLERSONLY_METHOD,
    //    nullptr,
    //    nullptr,
    //    (void**)&entry_point
    //);
    //if (rc != 0) {
    //    logger.info("Failed to get entry point from plugin management: {}\n", rc);
    //    return;
    //}
    //entry_point();
}
