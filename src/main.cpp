#include "dotnet.h"
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <filesystem>

void init_loading() {
    int rc = load_hostfxr();
    if (rc != 0) {
        std::cout << "Failed to load hostfxr: " << rc << std::endl;
        return;
    }
    rc = init_delegate_fptrs(LIBRARY_DIR_PATH MAIN_NAMESPACE
                             L"." PLUGIN_MANAGEMENT_NAME L".runtimeconfig.json");
    if (rc != 0) {
        std::cout << "Failed to get functions from hostfxr: " << rc << std::endl;
        return;
    }
    std::filesystem::path plugin_management_path =
        std::filesystem::absolute(LIBRARY_DIR_PATH MAIN_NAMESPACE "."
                                  PLUGIN_MANAGEMENT_NAME ".dll");
    rc = load_assembly_fptr(plugin_management_path.c_str(), nullptr, nullptr);
    if (rc != 0) {
        std::cout << "Failed to load plugin management: " << rc << std::endl;
        return;
    }
    typedef void(CORECLR_DELEGATE_CALLTYPE * entry_point_fn)();
    entry_point_fn entry_point = nullptr;
    rc = get_function_pointer_fptr(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main, " MAIN_NAMESPACE
                       L"." PLUGIN_MANAGEMENT_NAME,
        L"Initialize", UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr,
        (void **)&entry_point);
    if (rc != 0) {
        std::cout << "Failed to get entry point from plugin management: " << rc << std::endl;
        return;
    }
    entry_point();
}
