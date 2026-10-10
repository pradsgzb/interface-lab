#include "studydesk/example.hpp"
#include <algorithm>
#include <cctype>
#include <functional>

namespace ex19 {
// One rule has one job; the runner composes rules through this contract.
struct IValidationRule {
    virtual ~IValidationRule() = default;
    // Empty result means valid; otherwise return an explanation.
    virtual std::string error(const std::string& title) const = 0;
};
class Required final : public IValidationRule {
public:
    std::string error(const std::string& title) const override {
        const bool blank = std::all_of(title.begin(), title.end(),
                                       [](unsigned char c) { return std::isspace(c) != 0; });
        return blank ? "Title is required" : "";
    }
};
class MaxLength final : public IValidationRule {
public:
    std::string error(const std::string& title) const override {
        return title.size() > 20 ? "Title must be at most 20 bytes" : "";
    }
};
using Rules = std::vector<std::reference_wrapper<const IValidationRule>>;
std::string validate(const std::string& title, const Rules& rules) {
    for (const IValidationRule& rule : rules) {
        const auto message = rule.error(title);
        if (!message.empty()) {
            return message;
        } // First failure stops the pipeline.
    }
    return "";
}
void run(const lab::Context&) {
    Required required;
    MaxLength length;
    const Rules rules{required, length};
    lab::check(validate("   ", rules) == "Title is required", "Required rule rejects whitespace");
    lab::check(validate(std::string(21, 'x'), rules) == "Title must be at most 20 bytes",
               "Length rule rejects a long title");
    lab::check(validate("Read C++", rules).empty(), "All rules accept a valid title");
    lab::lesson(
        "Compose small validation policies; change the rule list without changing the runner.");
}
} // namespace ex19
