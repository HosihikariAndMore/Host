#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace hosihikari_host {

class HosihikariHost {
    HosihikariHost();

public:
    HosihikariHost(HosihikariHost&&)                 = delete;
    HosihikariHost(const HosihikariHost&)            = delete;
    HosihikariHost& operator=(HosihikariHost&&)      = delete;
    HosihikariHost& operator=(const HosihikariHost&) = delete;

    static HosihikariHost& getInstance();

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const;

    /// @return True if the plugin is loaded successfully.
    bool load(ll::plugin::NativePlugin&);

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

private:
    ll::plugin::NativePlugin* mSelf{};
};

} // namespace hosihikari_host
