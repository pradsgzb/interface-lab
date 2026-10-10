#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <fstream>
#include <sstream>

namespace ex06 {
// One task title per line. This toy format is suitable because TaskService
// rejects newlines. It is not a database or a concurrent storage system.
class TextTaskStore final : public studydesk::ITaskStore {
public:
    explicit TextTaskStore(std::filesystem::path path) : path_(std::move(path)) {
        std::ofstream create(path_, std::ios::app);
        if (!create) {
            throw std::runtime_error("Cannot create task file");
        }
    }
    void add(const studydesk::Task& task) override {
        std::ofstream file(path_, std::ios::app);
        file << task.title << '\n';
        file.flush();
        if (!file) {
            throw std::runtime_error("Cannot save task");
        }
    }
    std::vector<studydesk::Task> all() const override {
        std::ifstream file(path_);
        if (!file) {
            throw std::runtime_error("Cannot read tasks");
        }
        std::vector<studydesk::Task> tasks;
        std::string line;
        while (std::getline(file, line)) {
            tasks.push_back({line});
        }
        if (file.bad()) {
            throw std::runtime_error("Read failed");
        }
        return tasks;
    }

private:
    std::filesystem::path path_;
};
void run(const lab::Context& context) {
    const auto path = context.outputDirectory / "06-tasks.txt";
    {
        std::ofstream reset(path);
    } // Dedicated example output, reset for repeatable runs.
    studydesk::MemoryTaskStore memory;
    TextTaskStore disk(path);
    std::ostringstream messages;
    studydesk::StreamLogger logger(messages);
    for (studydesk::ITaskStore* store : std::vector<studydesk::ITaskStore*>{&memory, &disk}) {
        studydesk::TaskService service(*store, logger);
        service.addTask("Finish assignment");
        lab::check(service.tasks().size() == 1, "Same service works with this repository");
    }
    TextTaskStore reopened(path);
    lab::check(reopened.all().front().title == "Finish assignment",
               "File implementation persists after reopening");
    lab::lesson("Repository boundary: change storage without rewriting business rules.");
}
} // namespace ex06
