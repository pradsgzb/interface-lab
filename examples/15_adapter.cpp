#include "studydesk/example.hpp"

namespace ex15 {
// Imagine this is a third-party library that we cannot change.
class LegacyAttendanceApi {
public:
    int attendanceBasisPoints() const {
        return 8250;
    } // 10000 means 100%.
};
// The new client sees percentages regardless of the legacy API units.
struct IAttendance {
    virtual ~IAttendance() = default;
    virtual double percentage() const = 0;
};
class LegacyAttendanceAdapter final : public IAttendance {
public:
    explicit LegacyAttendanceAdapter(const LegacyAttendanceApi& legacy) : legacy_(legacy) {}
    double percentage() const override {
        return legacy_.attendanceBasisPoints() / 100.0;
    }

private:
    const LegacyAttendanceApi& legacy_;
};
bool meetsExamplePolicy(const IAttendance& attendance) {
    return attendance.percentage() >= 75.0;
}
void run(const lab::Context&) {
    LegacyAttendanceApi legacy;
    LegacyAttendanceAdapter adapter(legacy);
    lab::check(adapter.percentage() == 82.5 && meetsExamplePolicy(adapter),
               "Legacy units map to the new contract");
    lab::lesson("Adapter pattern: translate an incompatible API once at its boundary.");
    // Threshold is fictional for teaching, not an NSUT attendance policy.
}
} // namespace ex15
