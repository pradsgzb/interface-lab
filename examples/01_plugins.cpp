#include "studydesk/example.hpp"
#include "studydesk/plugin.hpp"
#include <memory>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace ex01 {
// OS-specific loading is isolated here; the rest of the app uses the contract.
class SharedLibrary {
public:
    explicit SharedLibrary(const std::filesystem::path& path) {
#if defined(_WIN32)
        handle_ = LoadLibraryW(std::filesystem::absolute(path).c_str());
        if (!handle_) {
            throw std::runtime_error("Cannot load plugin: " + path.string());
        }
#else
        handle_ = dlopen(std::filesystem::absolute(path).c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!handle_) {
            throw std::runtime_error(std::string("Cannot load plugin: ") + dlerror());
        }
#endif
    }
    ~SharedLibrary() {
#if defined(_WIN32)
        FreeLibrary(handle_);
#else
        dlclose(handle_);
#endif
    }
    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;
    template <typename Function> Function symbol(const char* name) const {
#if defined(_WIN32)
        auto address = GetProcAddress(handle_, name);
#else
        auto address = dlsym(handle_, name);
#endif
        if (!address) {
            throw std::runtime_error(std::string("Missing plugin symbol: ") + name);
        }
        // Platform loading APIs supply code addresses; this cast stays in the loader.
        return reinterpret_cast<Function>(address);
    }

private:
#if defined(_WIN32)
    HMODULE handle_ = nullptr;
#else
    void* handle_ = nullptr;
#endif
};
void run(const lab::Context& context) {
    SharedLibrary library(context.pluginPath); // Construct before the plugin.
    auto version = library.symbol<studydesk::PluginVersion>("study_plugin_version");
    lab::check(version() == 1, "Plugin contract version matches");
    auto create = library.symbol<studydesk::CreatePlugin>("create_study_plugin");
    auto destroy = library.symbol<studydesk::DestroyPlugin>("destroy_study_plugin");
    std::unique_ptr<studydesk::IStudyPlugin, studydesk::DestroyPlugin> plugin(create(), destroy);
    lab::check(plugin != nullptr, "Factory returned a plugin");
    std::cout << "  Loaded: " << plugin->name() << '\n';
    lab::check(plugin->recommendedMinutes(2) == 50, "Two sessions take 50 minutes");
    lab::lesson("Load new code at runtime; the host only knows IStudyPlugin.");
    // Reverse destruction order: plugin is destroyed BEFORE its library unloads.
}
} // namespace ex01
