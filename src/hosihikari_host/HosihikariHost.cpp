#include "HosihikariHost.h"

#include <memory>

#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"

#include "../main.h"

namespace hosihikari_host {

static std::unique_ptr<HosihikariHost> instance;

HosihikariHost& HosihikariHost::getInstance() { return *instance; }

bool HosihikariHost::load() {
    init_loading(getSelf().getLogger());
    return true;
}

bool HosihikariHost::enable() { return true; }

bool HosihikariHost::disable() { return true; }

} // namespace hosihikari_host

LL_REGISTER_PLUGIN(hosihikari_host::HosihikariHost, hosihikari_host::instance);
