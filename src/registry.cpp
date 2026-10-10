#include "studydesk/example.hpp"

namespace ex01 {
void run(const lab::Context&);
}
namespace ex02 {
void run(const lab::Context&);
}
namespace ex03 {
void run(const lab::Context&);
}
namespace ex04 {
void run(const lab::Context&);
}
namespace ex05 {
void run(const lab::Context&);
}
namespace ex06 {
void run(const lab::Context&);
}
namespace ex07 {
void run(const lab::Context&);
}
namespace ex08 {
void run(const lab::Context&);
}
namespace ex09 {
void run(const lab::Context&);
}
namespace ex10 {
void run(const lab::Context&);
}
namespace ex11 {
void run(const lab::Context&);
}
namespace ex12 {
void run(const lab::Context&);
}
namespace ex13 {
void run(const lab::Context&);
}
namespace ex14 {
void run(const lab::Context&);
}
namespace ex15 {
void run(const lab::Context&);
}
namespace ex16 {
void run(const lab::Context&);
}
namespace ex17 {
void run(const lab::Context&);
}
namespace ex18 {
void run(const lab::Context&);
}
namespace ex19 {
void run(const lab::Context&);
}
namespace ex20 {
void run(const lab::Context&);
}
namespace ex21 {
void run(const lab::Context&);
}

namespace lab {
const std::vector<Example>& examples() {
    static const std::vector<Example> entries{
        {1, "Runtime plugin / add-in", ex01::run},
        {2, "Add new app features", ex02::run},
        {3, "Console, file and memory loggers", ex03::run},
        {4, "CUI, GUI and web frontends", ex04::run},
        {5, "Strategy: revision plans", ex05::run},
        {6, "Repository: memory and file storage", ex06::run},
        {7, "Notification channels", ex07::run},
        {8, "Payment gateway simulation", ex08::run},
        {9, "Export formats", ex09::run},
        {10, "Hardware sensor abstraction", ex10::run},
        {11, "Clock injection", ex11::run},
        {12, "Small interfaces and multiple inheritance", ex12::run},
        {13, "Observer: task events", ex13::run},
        {14, "Decorator: composed logging", ex14::run},
        {15, "Adapter: a legacy API", ex15::run},
        {16, "Simple Factory: report selection", ex16::run},
        {17, "Command: undo an action", ex17::run},
        {18, "State: assignment workflow", ex18::run},
        {19, "Validation pipeline", ex19::run},
        {20, "Proxy: cached course catalog", ex20::run},
        {21, "Test doubles and failure paths", ex21::run},
    };
    return entries;
}
} // namespace lab
