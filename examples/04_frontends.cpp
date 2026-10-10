#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <sstream>

namespace ex04 {
void run(const lab::Context&) {
    studydesk::MemoryTaskStore store;
    std::ostringstream logs;
    studydesk::StreamLogger logger(logs);
    studydesk::TaskService service(store, logger);
    std::istringstream keyboard("add Revise interfaces\nlist\nquit\n");
    std::ostringstream screen;
    studydesk::ConsoleFrontend console(keyboard, screen);
    studydesk::IFrontend& frontend = console;
    frontend.run(service);
    std::cout << screen.str();
    lab::check(service.tasks().size() == 1, "CUI changes the shared service");
    lab::check(screen.str().find("- Revise interfaces") != std::string::npos,
               "CUI reads the same service");
    lab::lesson(
        "Run --cui, studydesk_gui, or studydesk_web: each uses IFrontend and ITaskService.");
    // See frontends/ for real desktop widgets and a real local HTTP application.
}
} // namespace ex04
