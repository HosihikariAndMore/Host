#pragma once

#include "hosihikari_host/HosihikariHost.h"

namespace hosihikari {
class PluginHandle {
public:
    void* mHandle;

    struct InteropArg {
        bool                       success;
        std::optional<std::string> error;

        static void __stdcall callback(InteropArg*, int, char const*);
    };

public:
    InteropArg load();
    InteropArg unload();
    InteropArg enable();
    InteropArg disable();
    InteropArg release_handle();

    using plugin_handle_callback_t = void(__stdcall)(InteropArg*, int, char const*);

private:
    using plugin_handle_method_fn = void(__stdcall*)(void*, InteropArg*, plugin_handle_callback_t*);

    static plugin_handle_method_fn mLoad;
    static plugin_handle_method_fn mEnable;
    static plugin_handle_method_fn mDisable;
    static plugin_handle_method_fn mUnload;
    static plugin_handle_method_fn mReleaseHandle;

    static bool mfptrInitialized;

public:
    static void initialize_methods();
};
} // namespace hosihikari