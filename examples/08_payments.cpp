#include "studydesk/example.hpp"

namespace ex08 {
// The provider returns a result; the caller owns the purchase decision.
struct IPaymentGateway {
    virtual ~IPaymentGateway() = default;
    virtual bool charge(int amountInPaise) = 0;
};
class ApprovedGateway final : public IPaymentGateway {
public:
    bool charge(int amountInPaise) override {
        return amountInPaise > 0;
    }
};
class DeclinedGateway final : public IPaymentGateway {
public:
    bool charge(int) override {
        return false;
    }
};
bool buyWorkshopSeat(IPaymentGateway& gateway) {
    return gateway.charge(50000); // Rs 500; integer minor units avoid floating-point money.
}
void run(const lab::Context&) {
    ApprovedGateway approved;
    DeclinedGateway declined;
    lab::check(buyWorkshopSeat(approved), "Approved result reserves the simulated seat");
    lab::check(!buyWorkshopSeat(declined), "Declined result does not reserve a seat");
    lab::lesson("Provider interface: handle success and failure without a real payment SDK.");
}
} // namespace ex08
