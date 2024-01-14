#include <ll/api/plugin/NativePlugin.h>

#include "Plugin.h"

#include "main.h"

namespace plugin {

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) {
    ll::Logger& logger = mSelf.getLogger();
    init_loading(logger);
}

bool Plugin::enable() {
    return true;
}

bool Plugin::disable() {
    return true;
}

} // namespace plugin
