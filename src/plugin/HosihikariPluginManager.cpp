#include "HosihikariPluginManager.h"
#include "PluginHandle.h"
#include "hosihikari_host/HosihikariHost.h"

namespace hosihikari {

static std::string errorMessage;
static void __stdcall errorHandler(char const* error) {
    if (error == nullptr) errorMessage = "";
    else errorMessage = error;
}

HosihikariPluginManager::HosihikariPluginManager() : ll::plugin::PluginManager("hosihikari") {}

HosihikariPluginManager::plugin_manager_method_fn HosihikariPluginManager::mLoadPlugin      = nullptr;
bool                                              HosihikariPluginManager::mfptrInitialized = false;

ll::Expected<> hosihikari::HosihikariPluginManager::load(ll::plugin::Manifest manifest) {

    if (hasPlugin(manifest.name)) {
        return ll::makeStringError(fmt::format("Plugin {0} already exists", (manifest.name)));
    }
    auto plugin = std::make_shared<HosihikariPlugin>(std::move(manifest));

    auto pluginDir = std::filesystem::canonical(ll::plugin::getPluginsRoot() / plugin->getManifest().name);

    if (plugin->getManifest().entry.empty())
        return ll::makeStringError(fmt::format("Plugin entry not found: {0}", pluginDir.string()));

    auto entry = pluginDir / plugin->getManifest().entry;

    if (!std::filesystem::exists(entry) or std::filesystem::is_directory(entry))
        return ll::makeStringError(fmt::format("Plugin entry not found: {0}", entry.string()));

    std::string name = plugin->getManifest().name;

    auto result = loadPlugin(entry.string().c_str());
    if (!result.first.success) return ll::makeStringError(result.first.error.value_or(""));
    plugin->getHandle() = result.second;

    plugin->onLoad([](ll::plugin::Plugin& plugin) -> bool {
        auto ret = reinterpret_cast<HosihikariPlugin&>(plugin).getHandle().load();
        if (!ret.success) {
            if (ret.error.has_value())
                hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error(ret.error.value());
            return false;
        } else {
            return true;
        }
    });
    plugin->onEnable([](ll::plugin::Plugin& plugin) -> bool {
        auto ret = reinterpret_cast<HosihikariPlugin&>(plugin).getHandle().enable();
        if (!ret.success) {
            if (ret.error.has_value())
                hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error(ret.error.value());
            return false;
        } else {
            return true;
        }
    });
    plugin->onDisable([](ll::plugin::Plugin& plugin) -> bool {
        auto ret = reinterpret_cast<HosihikariPlugin&>(plugin).getHandle().disable();
        if (!ret.success) {
            if (ret.error.has_value())
                hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error(ret.error.value());
            return false;
        } else {
            return true;
        }
    });
    plugin->onUnload([](ll::plugin::Plugin& plugin) -> bool {
        auto ret = reinterpret_cast<HosihikariPlugin&>(plugin).getHandle().unload();
        if (!ret.success) {
            if (ret.error.has_value())
                hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error(ret.error.value());
            return false;
        } else {
            return true;
        }
    });

    return plugin->onLoad().transform([this, pl = plugin] { addPlugin(pl->getManifest().name, pl); });
}

ll::Expected<> hosihikari::HosihikariPluginManager::unload(std::string_view name) {
    auto temp = std::static_pointer_cast<HosihikariPlugin>(getPlugin(name));
    if (temp) return temp->onUnload();
    return {};
}

std::pair<PluginHandle::InteropArg, PluginHandle> HosihikariPluginManager::loadPlugin(char const* path) {
    if (mfptrInitialized) {
        PluginHandle::InteropArg arg{};
        void*                    handle;
        mLoadPlugin(path, &handle, &arg, PluginHandle::InteropArg::callback);
        return {arg, {handle}};
    }
    hosihikari::host::HosihikariHost::getInstance().getSelf().getLogger().error("Plugin handle not initialized");

    return {};
}

void HosihikariPluginManager::initialize_methods() {
    auto& host = hosihikari::host::HosihikariHost::getInstance();
    auto  rc   = host.getFunctionPointerFptr()(
        MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAMESPACE L".Main, " MAIN_NAMESPACE L"." PLUGIN_MANAGEMENT_NAME,
        L"LoadPluginUnmanaged",
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        nullptr,
        (void**)&mLoadPlugin
    );

    if (rc != 0) {
        host.getSelf().getLogger().error("Failed to initialize plugin handle");
        return;
    }

    mfptrInitialized = true;
}

} // namespace hosihikari
