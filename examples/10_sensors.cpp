#include "studydesk/example.hpp"

namespace ex10 {
struct ITemperatureSensor {
    virtual ~ITemperatureSensor() = default;
    virtual double celsius() const = 0; // The UNIT is part of the contract.
};
class CelsiusSensor final : public ITemperatureSensor {
public:
    double celsius() const override {
        return 30.0;
    } // Simulated hardware reading.
};
class FahrenheitSensor final : public ITemperatureSensor {
public:
    double celsius() const override {
        return (86.0 - 32.0) * 5.0 / 9.0;
    }
};
bool needsCooling(const ITemperatureSensor& sensor) {
    return sensor.celsius() >= 28.0;
}
void run(const lab::Context&) {
    CelsiusSensor native;
    FahrenheitSensor converted;
    lab::check(needsCooling(native) && needsCooling(converted),
               "Cooling rule works with both simulated devices");
    lab::lesson("Hardware abstraction: normalize units at the boundary, not in every caller.");
}
} // namespace ex10
