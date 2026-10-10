#include "studydesk/core.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

namespace studydesk {
void MemoryTaskStore::add(const Task& task) {
    tasks_.push_back(task);
}
std::vector<Task> MemoryTaskStore::all() const {
    return tasks_;
}
void StreamLogger::log(const std::string& message) {
    output_ << "[log] " << message << '\n';
}

void TaskService::addTask(const std::string& title) {
    const bool blank = std::all_of(title.begin(), title.end(),
                                   [](unsigned char c) { return std::isspace(c) != 0; });
    const bool control =
        std::any_of(title.begin(), title.end(), [](unsigned char c) { return c < 32 || c == 127; });
    if (title.empty() || blank || title.size() > 80 || control) {
        throw std::invalid_argument("Use 1-80 bytes of visible text (no control characters).");
    }
    store_.add(Task{title});
    logger_.log("Added: " + title);
}
std::vector<Task> TaskService::tasks() const {
    return store_.all();
}

int ConsoleFrontend::run(ITaskService& service) {
    output_ << "StudyDesk CUI | add <title>, list, quit\n";
    std::string line;
    while (output_ << "> " && std::getline(input_, line)) {
        if (line == "quit") {
            return 0;
        }
        if (line == "list") {
            const auto tasks = service.tasks();
            if (tasks.empty()) {
                output_ << "No tasks yet.\n";
            }
            for (const auto& task : tasks) {
                output_ << "- " << task.title << '\n';
            }
        } else if (line.rfind("add ", 0) == 0) {
            try {
                service.addTask(line.substr(4));
                output_ << "Added.\n";
            } catch (const std::invalid_argument& error) {
                output_ << "Cannot add: " << error.what() << '\n';
            }
        } else {
            output_ << "Commands: add <title>, list, quit\n";
        }
    }
    return 0; // EOF also exits cleanly, so this frontend can be scripted.
}
} // namespace studydesk
