#include "PluginHandle.h"

namespace hosihikari {

bool                                  PluginHandle::mfptrInitialized = false;
PluginHandle::plugin_handle_method_fn PluginHandle::mLoad            = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mUnload          = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mEnable          = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mDisable         = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mReleaseHandle   = nullptr;

void __stdcall PluginHandle::InteropArg::callback(InteropArg* arg, int success, char const* error) {
    arg->success = (bool)success;
    if (!arg->success) arg->error = error;
}

PluginHandle::InteropArg PluginHandle::load() {
    if (!mfptrInitialized) {
        hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized");
        return {false, {}};
    }
    auto arg = InteropArg{};
    mLoad(mHandle, &arg, InteropArg::callback);
    return arg;
}

PluginHandle::InteropArg PluginHandle::unload() {
    if (!mfptrInitialized) {
        hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized");
        return {false, {}};
    }
    auto arg = InteropArg{};
    mUnload(mHandle, &arg, InteropArg::callback);
    return arg;
}

PluginHandle::InteropArg PluginHandle::enable() {
    if (!mfptrInitialized) {
        hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized");
        return {false, {}};
    }
    auto arg = InteropArg{};
    mEnable(mHandle, &arg, InteropArg::callback);
    return arg;
}

PluginHandle::InteropArg PluginHandle::disable() {
    if (!mfptrInitialized) {
        hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized");
        return {false, {}};
    }
    auto arg = InteropArg{};
    mDisable(mHandle, &arg, InteropArg::callback);
    return arg;
}

PluginHandle::InteropArg PluginHandle::release_handle() {
    if (!mfptrInitialized) {
        hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized");
        return {false, {}};
    }
    auto arg = InteropArg{};
    mReleaseHandle(mHandle, &arg, InteropArg::callback);
    return arg;
}

void PluginHandle::initialize_methods() {

    auto& host = hosihikari::host::HosihikariHost::getInstance();

    auto rc = host.getFunctionPointerFptr()(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L"." PLUGIN_MANAGEMENT_CLASS_NAME L","
                                                                                          L" " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
        L"Load",
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        nullptr,
        (void**)&mLoad
    );

    if (rc != 0) {
        host.getSelf().getLogger().error("Failed to initialize plugin handle");
        return;
    }

    rc = host.getFunctionPointerFptr()(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L"." PLUGIN_MANAGEMENT_CLASS_NAME L","
                                                                                          L" " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
        L"Unload",
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        nullptr,
        (void**)&mUnload
    );

    if (rc != 0) {
        host.getSelf().getLogger().error("Failed to initialize plugin handle");
        return;
    }

    rc = host.getFunctionPointerFptr()(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L"." PLUGIN_MANAGEMENT_CLASS_NAME L","
                                                                                          L" " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
        L"Enable",
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        nullptr,
        (void**)&mEnable
    );

    if (rc != 0) {
        host.getSelf().getLogger().error("Failed to initialize plugin handle");
        return;
    }

    rc = host.getFunctionPointerFptr()(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L"." PLUGIN_MANAGEMENT_CLASS_NAME L","
                                                                                          L" " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
        L"Disable",
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        nullptr,
        (void**)&mDisable
    );

    if (rc != 0) {
        host.getSelf().getLogger().error("Failed to initialize plugin handle");
        return;
    }

    rc = host.getFunctionPointerFptr()(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L"." PLUGIN_MANAGEMENT_CLASS_NAME L","
                                                                                          L" " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
        L"ReleaseHandle",
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        nullptr,
        (void**)&mReleaseHandle
    );

    if (rc != 0) {
        host.getSelf().getLogger().error("Failed to initialize plugin handle");
        return;
    }

    PluginHandle::mfptrInitialized = true;
}

} // namespace hosihikari
