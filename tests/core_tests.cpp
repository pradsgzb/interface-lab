#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <sstream>

int main() {
    try {
        studydesk::MemoryTaskStore store;
        std::ostringstream logs;
        studydesk::StreamLogger logger(logs);
        studydesk::TaskService service(store, logger);
        for (const std::string& title :
             std::vector<std::string>{"", "   ", "bad\nline", std::string(81, 'a')}) {
            bool rejected = false;
            try {
                service.addTask(title);
            } catch (const std::invalid_argument&) {
                rejected = true;
            }
            lab::check(rejected, "Service rejects invalid input");
        }
        lab::check(store.all().empty() && logs.str().empty(),
                   "Invalid input has no storage or logging side effects");
        service.addTask(std::string(80, 'a'));
        auto snapshot = service.tasks();
        snapshot.clear();
        lab::check(service.tasks().size() == 1,
                   "Caller cannot mutate storage through a returned snapshot");
        std::istringstream commands("add    \nadd Practice C++\nlist\nunknown\nquit\n");
        std::ostringstream screen;
        studydesk::ConsoleFrontend console(commands, screen);
        console.run(service);
        lab::check(service.tasks().size() == 2, "Console uses service validation");
        lab::check(screen.str().find("- Practice C++") != std::string::npos,
                   "Console shows added task");
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
