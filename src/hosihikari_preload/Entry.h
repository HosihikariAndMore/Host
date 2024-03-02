#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace hosihikari_host {

[[nodiscard]] auto getSelfPluginInstance() -> ll::plugin::NativePlugin&;

} // namespace hosihikari_host
