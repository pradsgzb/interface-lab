#include "studydesk/core.hpp"
#include "studydesk/example.hpp"

namespace ex21 {
class RecordingLogger final : public studydesk::ILogger {
public:
    void log(const std::string& message) override {
        messages.push_back(message);
    }
    std::vector<std::string> messages;
};
class UnavailableStore final : public studydesk::ITaskStore {
public:
    void add(const studydesk::Task&) override {
        throw std::runtime_error("Disk unavailable");
    }
    std::vector<studydesk::Task> all() const override {
        return {};
    }
};
void run(const lab::Context&) {
    UnavailableStore unavailable;
    RecordingLogger logger;
    studydesk::TaskService failing(unavailable, logger);
    bool errorReachedCaller = false;
    try {
        failing.addTask("Read C++");
    } catch (const std::runtime_error&) {
        errorReachedCaller = true;
    }
    lab::check(errorReachedCaller, "Storage failure reaches the caller");
    lab::check(logger.messages.empty(), "Failed save does not emit a success log");
    studydesk::MemoryTaskStore memory;
    studydesk::TaskService working(memory, logger);
    working.addTask("Read C++");
    lab::check(logger.messages.size() == 1 && logger.messages.front() == "Added: Read C++",
               "Successful save logs exactly once");
    lab::lesson("Inject test doubles to reproduce failures and observe collaboration without "
                "external systems.");
}
} // namespace ex21
