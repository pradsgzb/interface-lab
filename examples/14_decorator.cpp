#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <sstream>

namespace ex14 {
class PrefixLogger final : public studydesk::ILogger {
public:
    PrefixLogger(studydesk::ILogger& next, std::string prefix)
        : next_(next), prefix_(std::move(prefix)) {}
    void log(const std::string& message) override {
        next_.log(prefix_ + message);
    }

private:
    studydesk::ILogger& next_;
    std::string prefix_;
};
void run(const lab::Context&) {
    std::ostringstream output;
    studydesk::StreamLogger base(output);
    PrefixLogger course(base, "[CS] ");
    PrefixLogger student(course, "[Aarav] ");
    studydesk::ILogger& logger = student;
    logger.log("Submitted");
    lab::check(output.str() == "[log] [CS] [Aarav] Submitted\n",
               "Two wrappers compose around one logger");
    std::cout << output.str();
    lab::lesson("Decorator pattern: add behavior without editing the wrapped class.");
}
} // namespace ex14
