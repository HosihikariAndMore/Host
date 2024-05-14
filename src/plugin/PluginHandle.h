#pragma once

#include "hosihikari_host/HosihikariHost.h"

namespace hosihikari {
class PluginHandle {
public:
    void* mHandle;

    struct InteropArg {
        bool                       success;
        std::optional<std::string> error;

        static void __stdcall callback(InteropArg* arg, int success, char const* error) {
            arg->success = (bool)success;
            if (!arg->success) arg->error = error;
        }
    };

public:
    InteropArg load() {
        if (!mfptrInitialized) {
            hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized"
            );
            return {false, {}};
        }
        auto arg = InteropArg{};
        mLoad(mHandle, &arg, InteropArg::callback);
        return arg;
    }
    InteropArg unload() {
        if (!mfptrInitialized) {
            hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized"
            );
            return {false, {}};
        }
        auto arg = InteropArg{};
        mUnload(mHandle, &arg, InteropArg::callback);
        return arg;
    }
    InteropArg enable() {
        if (!mfptrInitialized) {
            hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized"
            );
            return {false, {}};
        }
        auto arg = InteropArg{};
        mEnable(mHandle, &arg, InteropArg::callback);
        return arg;
    }
    InteropArg disable() {
        if (!mfptrInitialized) {
            hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized"
            );
            return {false, {}};
        }
        auto arg = InteropArg{};
        mDisable(mHandle, &arg, InteropArg::callback);
        return arg;
    }
    InteropArg release_handle() {
        if (!mfptrInitialized) {
            hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized"
            );
            return {false, {}};
        }
        auto arg = InteropArg{};
        mReleaseHandle(mHandle, &arg, InteropArg::callback);
        return arg;
    }


private:
    using plugin_handle_callback_t = void(__stdcall)(InteropArg*, int, char const*);

    using plugin_handle_method_fn = void(__stdcall*)(void*, InteropArg*, plugin_handle_callback_t*);

    static plugin_handle_method_fn mLoad;
    static plugin_handle_method_fn mEnable;
    static plugin_handle_method_fn mDisable;
    static plugin_handle_method_fn mUnload;
    static plugin_handle_method_fn mReleaseHandle;

    static bool mfptrInitialized;

public:
    static void initialize_methods() {

        auto& host = hosihikari::host::HosihikariHost::getInstance();

        auto rc = host.getFunctionPointerFptr()(
            MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main,"
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
            MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main,"
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
            MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main,"
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
            MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main,"
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
            MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main,"
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
};
} // namespace hosihikari