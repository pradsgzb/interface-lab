#include "studydesk/example.hpp"
#include <chrono>

namespace ex11 {
using Time = std::chrono::system_clock::time_point;
// Time comes from a dependency so boundary cases can be reproduced.
struct IClock {
    virtual ~IClock() = default;
    virtual Time now() const = 0;
};
class SystemClock final : public IClock {
public:
    Time now() const override {
        return std::chrono::system_clock::now();
    }
};
class FixedClock final : public IClock {
public:
    explicit FixedClock(Time time) : time_(time) {}
    Time now() const override {
        return time_;
    }

private:
    Time time_;
};
bool isOverdue(Time deadline, const IClock& clock) {
    return clock.now() > deadline;
}
void run(const lab::Context&) {
    const Time deadline = Time{} + std::chrono::hours(24);
    FixedClock before(deadline - std::chrono::seconds(1));
    FixedClock exact(deadline);
    FixedClock after(deadline + std::chrono::seconds(1));
    lab::check(!isOverdue(deadline, before), "Before deadline: not overdue");
    lab::check(!isOverdue(deadline, exact), "At deadline: not overdue by our rule");
    lab::check(isOverdue(deadline, after), "After deadline: overdue");
    lab::lesson(
        "Inject time to test boundaries instantly, without sleeping or changing the system clock.");
}
} // namespace ex11
