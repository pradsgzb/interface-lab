#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <sstream>

namespace ex09 {
// Input is domain data; each implementation owns one output format.
struct ITaskExporter {
    virtual ~ITaskExporter() = default;
    virtual std::string render(const std::vector<studydesk::Task>& tasks) const = 0;
};
class PlainTextExporter final : public ITaskExporter {
public:
    std::string render(const std::vector<studydesk::Task>& tasks) const override {
        std::ostringstream output;
        for (const auto& task : tasks) {
            output << "- " << task.title << '\n';
        }
        return output.str();
    }
};
class CsvExporter final : public ITaskExporter {
public:
    std::string render(const std::vector<studydesk::Task>& tasks) const override {
        std::ostringstream output;
        output << "title\n";
        for (const auto& task : tasks) {
            output << '"';
            for (char c : task.title) {
                if (c == '"') {
                    output << '"';
                } // CSV quotes are doubled.
                output << c;
            }
            output << "\"\n";
        }
        return output.str();
    }
};
std::string exportTasks(const ITaskExporter& exporter, const std::vector<studydesk::Task>& tasks) {
    return exporter.render(tasks);
}
void run(const lab::Context&) {
    const std::vector<studydesk::Task> tasks{{"Read \"Interfaces\", chapter 1"}};
    PlainTextExporter text;
    CsvExporter csv;
    lab::check(exportTasks(text, tasks) == "- Read \"Interfaces\", chapter 1\n",
               "Text exporter formats the task");
    lab::check(exportTasks(csv, tasks) == "title\n\"Read \"\"Interfaces\"\", chapter 1\"\n",
               "CSV exporter escapes commas and quotes");
    lab::lesson("Keep data separate from the representation chosen by the user.");
}
} // namespace ex09
