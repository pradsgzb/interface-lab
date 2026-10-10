#include "studydesk/example.hpp"
#include <memory>
#include <sstream>

namespace ex02 {
struct IFeature {
    virtual ~IFeature() = default;
    virtual void execute(std::ostream& output) const = 0;
};
class Timetable final : public IFeature {
public:
    void execute(std::ostream& output) const override {
        output << "09:00 Data structures\n";
    }
};
class ExamCountdown final : public IFeature {
public:
    void execute(std::ostream& output) const override {
        output << "Exam in 7 days\n";
    }
};
class StudyApp {
public:
    void install(std::unique_ptr<IFeature> feature) {
        features_.push_back(std::move(feature));
    }
    void run(std::ostream& output) const {
        // This loop never changes when a new feature is registered.
        for (const auto& feature : features_) {
            feature->execute(output);
        }
    }

private:
    std::vector<std::unique_ptr<IFeature>> features_;
};
void run(const lab::Context&) {
    StudyApp app;
    app.install(std::make_unique<Timetable>());
    app.install(std::make_unique<ExamCountdown>()); // Only the wiring changes.
    std::ostringstream result;
    app.run(result);
    std::cout << result.str();
    lab::check(result.str().find("Exam in 7 days") != std::string::npos,
               "New feature runs through the original app loop");
    lab::lesson("Open/Closed Principle: add an implementation without editing the stable client.");
}
} // namespace ex02
