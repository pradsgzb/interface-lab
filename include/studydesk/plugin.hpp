#pragma once

namespace studydesk {
// Versioned, intentionally tiny C++ contract. No STL types cross this boundary.
// This is NOT a cross-compiler ABI. Rebuild host and plugins with the same
// compiler, C++ runtime, architecture, configuration, and header version.
struct IStudyPlugin {
    virtual ~IStudyPlugin() = default;
    virtual const char* name() const noexcept = 0;
    virtual int recommendedMinutes(int sessions) const noexcept = 0;
};
using PluginVersion = int (*)();
using CreatePlugin = IStudyPlugin* (*)();
using DestroyPlugin = void (*)(IStudyPlugin*);
} // namespace studydesk

#if defined(_WIN32)
#define STUDY_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define STUDY_PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif
