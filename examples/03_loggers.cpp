#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <fstream>
#include <sstream>

namespace ex03 {
class FileLogger final : public studydesk::ILogger {
public:
    explicit FileLogger(const std::filesystem::path& path) : file_(path) {
        if (!file_) {
            throw std::runtime_error("Cannot open log file");
        }
    }
    void log(const std::string& message) override {
        file_ << message << '\n';
        file_.flush();
        if (!file_) {
            throw std::runtime_error("Cannot write log file");
        }
    }

private:
    std::ofstream file_;
};
class MemoryLogger final : public studydesk::ILogger {
public:
    void log(const std::string& message) override {
        messages.push_back(message);
    }
    std::vector<std::string> messages;
};
// The same client works with all three destinations.
void startSession(studydesk::ILogger& logger) {
    logger.log("Study session started");
}
void run(const lab::Context& context) {
    studydesk::StreamLogger console(std::cout);
    FileLogger file(context.outputDirectory / "03-session.log");
    MemoryLogger memory;
    startSession(console);
    startSession(file);
    startSession(memory);
    std::ifstream saved(context.outputDirectory / "03-session.log");
    std::string line;
    std::getline(saved, line);
    lab::check(memory.messages.size() == 1 && line == memory.messages.front(),
               "File and memory loggers receive the same event");
    lab::lesson("Change the log destination without changing the caller.");
}
} // namespace ex03
