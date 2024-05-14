#include "HosihikariPlugin.h"

#include <ll/api/plugin/Plugin.h>

namespace hosihikari {
HosihikariPlugin::HosihikariPlugin(const ll::plugin::Manifest& manifest) : ll::plugin::Plugin(manifest), mHandle() {}
} // namespace hosihikari