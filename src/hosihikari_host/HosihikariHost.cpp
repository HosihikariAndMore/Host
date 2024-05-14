#include "HosihikariHost.h"

#include <memory>

#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/plugin/PluginManagerRegistry.h>
#include <ll/api/plugin/RegisterHelper.h>

#include "../main.h"
#include "../plugin/HosihikariPluginManager.h"

namespace hosihikari {
namespace host {

static std::unique_ptr<HosihikariHost> instance;

HosihikariHost& HosihikariHost::getInstance() { return *instance; }

bool HosihikariHost::load() const {
    auto& logger = getSelf().getLogger();
    init_loading(logger);
    hosihikari::HosihikariPluginManager::initialize_methods();
    hosihikari::PluginHandle::initialize_methods();
    auto  manager               = std::make_shared<hosihikari::HosihikariPluginManager>();
    auto& pluginManagerRegistry = ll::plugin::PluginManagerRegistry::getInstance();
    if (!pluginManagerRegistry.addManager(manager)) {
        logger.error("Failed to add plugin manager.");
        return false;
    }
    return true;
}

bool HosihikariHost::enable() { return true; }

bool HosihikariHost::disable() { return true; }

get_function_pointer_fn HosihikariHost::getFunctionPointerFptr() const { return get_function_pointer_fptr; }

} // namespace host
} // namespace hosihikari

LL_REGISTER_PLUGIN(hosihikari::host::HosihikariHost, hosihikari::host::instance);
