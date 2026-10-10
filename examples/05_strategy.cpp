#include "studydesk/example.hpp"

namespace ex05 {
// An algorithm is the variation point: the planner borrows its strategy.
struct IRevisionStrategy {
    virtual ~IRevisionStrategy() = default;
    virtual int minutesPerChapter() const = 0;
};
class RegularRevision final : public IRevisionStrategy {
public:
    int minutesPerChapter() const override {
        return 30;
    }
};
class QuickRevision final : public IRevisionStrategy {
public:
    int minutesPerChapter() const override {
        return 10;
    }
};
int planMinutes(int chapters, const IRevisionStrategy& strategy) {
    return chapters * strategy.minutesPerChapter();
}
void run(const lab::Context&) {
    RegularRevision regular;
    QuickRevision quick;
    lab::check(planMinutes(3, regular) == 90, "Regular strategy: 90 minutes");
    lab::check(planMinutes(3, quick) == 30, "Quick strategy: 30 minutes");
    lab::lesson("Strategy pattern: swap an algorithm while keeping its caller unchanged.");
}
} // namespace ex05
