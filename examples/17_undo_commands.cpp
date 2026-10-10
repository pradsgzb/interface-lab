#include "studydesk/example.hpp"
#include <optional>

namespace ex17 {
// The invoker sees an action, while the command remembers what changed.
struct ICommand {
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};
class RenameTask final : public ICommand {
public:
    RenameTask(std::string& title, std::string replacement)
        : title_(title), replacement_(std::move(replacement)) {}
    void execute() override {
        if (oldTitle_) {
            throw std::logic_error("Undo before executing again");
        }
        oldTitle_ = title_;
        title_ = replacement_;
    }
    void undo() override {
        if (!oldTitle_) {
            throw std::logic_error("Nothing to undo");
        }
        title_ = *oldTitle_;
        oldTitle_.reset();
    }

private:
    std::string& title_;
    std::string replacement_;
    std::optional<std::string> oldTitle_;
};
void run(const lab::Context&) {
    std::string title = "Read";
    RenameTask rename(title, "Read chapter 1");
    ICommand& command = rename; // A menu or toolbar would also use ICommand.
    command.execute();
    lab::check(title == "Read chapter 1", "Command changes the title");
    command.undo();
    lab::check(title == "Read", "Undo restores the previous title");
    lab::lesson("Command pattern: make an action an object that can be invoked and undone.");
}
} // namespace ex17
