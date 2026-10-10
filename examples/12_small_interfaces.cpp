#include "studydesk/example.hpp"

namespace ex12 {
// Separate contracts avoid forcing unsupported operations on simple devices.
struct IPrinter {
    virtual ~IPrinter() = default;
    virtual std::string print(const std::string& text) = 0;
};
struct IScanner {
    virtual ~IScanner() = default;
    virtual std::string scan() const = 0;
};
class SimplePrinter final : public IPrinter {
public:
    std::string print(const std::string& text) override {
        return "Printed: " + text;
    }
};
// Multiple inheritance of SMALL contracts, not shared mutable base-class state.
class AllInOne final : public IPrinter, public IScanner {
public:
    std::string print(const std::string& text) override {
        return "Printed: " + text;
    }
    std::string scan() const override {
        return "Scanned class notes";
    }
};
std::string printNotes(IPrinter& printer) {
    return printer.print("C++ interfaces");
}
void run(const lab::Context&) {
    SimplePrinter printer;
    AllInOne device;
    lab::check(printNotes(printer) == printNotes(device), "Printing client accepts both devices");
    const IScanner& scanner = device;
    lab::check(scanner.scan() == "Scanned class notes", "Scanning client requests only IScanner");
    lab::lesson("Interface Segregation: a printer should not pretend to scan.");
}
} // namespace ex12
