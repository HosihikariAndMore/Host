#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace hosihikari_preload {

[[nodiscard]] auto getSelfPluginInstance() -> ll::plugin::NativePlugin&;

} // namespace hosihikari_preload
