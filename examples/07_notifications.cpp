#include "studydesk/example.hpp"

namespace ex07 {
// The reminder knows this delivery contract, not a channel implementation.
struct INotifier {
    virtual ~INotifier() = default;
    virtual std::string send(const std::string& message) = 0;
};
// Simulated delivery: no messages leave the program.
class EmailPreview final : public INotifier {
public:
    std::string send(const std::string& message) override {
        return "Email preview: " + message;
    }
};
class DesktopPreview final : public INotifier {
public:
    std::string send(const std::string& message) override {
        return "Desktop preview: " + message;
    }
};
std::string remind(INotifier& notifier) {
    return notifier.send("Assignment due tomorrow");
}
void run(const lab::Context&) {
    EmailPreview email;
    DesktopPreview desktop;
    lab::check(remind(email) == "Email preview: Assignment due tomorrow",
               "Email adapter receives reminder");
    lab::check(remind(desktop) == "Desktop preview: Assignment due tomorrow",
               "Desktop adapter receives reminder");
    lab::lesson("One reminder workflow, many delivery channels; delivery is simulated here.");
}
} // namespace ex07
