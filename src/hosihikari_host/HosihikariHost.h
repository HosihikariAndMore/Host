#pragma once

#include <dotnet.h>

#include "ll/api/plugin/NativePlugin.h"

namespace hosihikari {
namespace host {

class HosihikariHost {

public:
    static HosihikariHost& getInstance();

    HosihikariHost(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    /// @return True if the plugin is loaded successfully.
    bool load() const;

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the plugin.
    // /// @return True if the plugin is unloaded successfully.
    // bool unload();

public:
    get_function_pointer_fn getFunctionPointerFptr() const;

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace host
} // namespace hosihikari