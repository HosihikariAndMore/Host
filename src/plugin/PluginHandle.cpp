#include "PluginHandle.h"

namespace hosihikari {

bool                                  PluginHandle::mfptrInitialized = false;
PluginHandle::plugin_handle_method_fn PluginHandle::mLoad            = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mUnload          = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mEnable          = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mDisable         = nullptr;
PluginHandle::plugin_handle_method_fn PluginHandle::mReleaseHandle   = nullptr;

} // namespace hosihikari
