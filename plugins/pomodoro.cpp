#include "studydesk/plugin.hpp"

namespace {
class PomodoroPlugin final : public studydesk::IStudyPlugin {
public:
    const char* name() const noexcept override {
        return "Pomodoro add-in";
    }
    int recommendedMinutes(int sessions) const noexcept override {
        return sessions > 0 && sessions <= 100 ? sessions * 25 : 0;
    }
};
} // namespace
// C linkage makes factory symbol names predictable. It does NOT turn the C++
// object contract above into a stable, cross-compiler C ABI.
STUDY_PLUGIN_EXPORT int study_plugin_version() {
    return 1;
}
STUDY_PLUGIN_EXPORT studydesk::IStudyPlugin* create_study_plugin() {
    return new PomodoroPlugin();
}
STUDY_PLUGIN_EXPORT void destroy_study_plugin(studydesk::IStudyPlugin* plugin) {
    delete plugin; // Allocation and destruction happen in the same module.
}
