#include "HosihikariHost.h"

#include <ll/api/plugin/NativePlugin.h>
#include <memory>

#include "src/main.h"

namespace hosihikari_host {

HosihikariHost::HosihikariHost() = default;

HosihikariHost& HosihikariHost::getInstance() {
    static HosihikariHost instance;
    return instance;
}

ll::plugin::NativePlugin& HosihikariHost::getSelf() const { return *mSelf; }

bool HosihikariHost::load(ll::plugin::NativePlugin& self) {
    mSelf = std::addressof(self);

    init_loading(getSelf().getLogger());

    return true;
}

bool HosihikariHost::enable() { return true; }

bool HosihikariHost::disable() { return true; }

extern "C" {
_declspec(dllexport) bool ll_plugin_load(ll::plugin::NativePlugin& self) {
    return HosihikariHost::getInstance().load(self);
}

_declspec(dllexport) bool ll_plugin_enable(ll::plugin::NativePlugin&) { return HosihikariHost::getInstance().enable(); }

_declspec(dllexport) bool ll_plugin_disable(ll::plugin::NativePlugin&) {
    return HosihikariHost::getInstance().disable();
}

/// @warning Unloading the plugin may cause a crash if the plugin has not released all of its
/// resources. If you are unsure, keep this function commented out.
// _declspec(dllexport) bool ll_plugin_unload(ll::plugin::NativePlugin&) {
//     return HosihikariHost::getInstance().unload();
// }
}

} // namespace hosihikari_host
