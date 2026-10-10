#include "studydesk/example.hpp"

namespace ex18 {
// Each state supplies the behavior for one phase of the workflow.
struct ISubmissionState {
    virtual ~ISubmissionState() = default;
    virtual bool canEdit() const = 0;
    virtual std::string label() const = 0;
};
class Draft final : public ISubmissionState {
public:
    bool canEdit() const override {
        return true;
    }
    std::string label() const override {
        return "Draft";
    }
};
class Submitted final : public ISubmissionState {
public:
    bool canEdit() const override {
        return false;
    }
    std::string label() const override {
        return "Submitted";
    }
};
class Assignment {
public:
    Assignment() : state_(&draft_) {}
    Assignment(const Assignment&) = delete; // state_ points into this object.
    Assignment& operator=(const Assignment&) = delete;
    bool edit(const std::string& text) {
        if (!state_->canEdit()) {
            return false;
        }
        text_ = text;
        return true;
    }
    void submit() {
        state_ = &submitted_;
    }
    std::string status() const {
        return state_->label();
    }

private:
    Draft draft_;
    Submitted submitted_;
    const ISubmissionState* state_;
    std::string text_;
};
void run(const lab::Context&) {
    Assignment assignment;
    lab::check(assignment.edit("My solution"), "Draft accepts edits");
    assignment.submit();
    lab::check(!assignment.edit("Late edit") && assignment.status() == "Submitted",
               "Submitted state locks editing");
    lab::lesson("State pattern: delegate state-dependent behavior through one contract.");
}
} // namespace ex18
