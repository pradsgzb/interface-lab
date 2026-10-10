#include "studydesk/example.hpp"
#include <memory>

namespace ex16 {
// Clients consume IReport; only the factory selects concrete classes.
struct IReport {
    virtual ~IReport() = default;
    virtual std::string generate() const = 0;
};
class BriefReport final : public IReport {
public:
    std::string generate() const override {
        return "3 tasks completed";
    }
};
class DetailedReport final : public IReport {
public:
    std::string generate() const override {
        return "3 tasks completed: C++, Maths, OS";
    }
};
// Simple Factory (not the GoF Factory Method pattern).
std::unique_ptr<IReport> makeReport(const std::string& kind) {
    if (kind == "brief") {
        return std::make_unique<BriefReport>();
    }
    if (kind == "detailed") {
        return std::make_unique<DetailedReport>();
    }
    throw std::invalid_argument("Unknown report kind");
}
void run(const lab::Context&) {
    auto brief = makeReport("brief");
    auto detailed = makeReport("detailed");
    lab::check(brief->generate() == "3 tasks completed", "Factory selects brief report");
    lab::check(detailed->generate().find("Maths") != std::string::npos,
               "Factory selects detailed report");
    bool rejected = false;
    try {
        auto unknown = makeReport("unknown");
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    lab::check(rejected, "Unknown configuration is rejected");
    lab::lesson("Separate creation from use; selection logic belongs in the factory.");
}
} // namespace ex16
