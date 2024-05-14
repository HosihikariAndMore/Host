#pragma once

#include <ll/api/plugin/Manifest.h>
#include <ll/api/plugin/Plugin.h>

#include "PluginHandle.h"

namespace hosihikari {

class HosihikariPluginManager;

class HosihikariPlugin : public ll::plugin::Plugin {
    friend class ll::plugin::PluginManager;
    friend class HosihikariPluginManager;

public:
    HosihikariPlugin(const ll::plugin::Manifest& manifest);

private:
    PluginHandle mHandle;

public:
    PluginHandle& getHandle() { return mHandle; }
};
} // namespace hosihikari