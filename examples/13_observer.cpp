#include "studydesk/example.hpp"
#include <functional>

namespace ex13 {
// The publisher announces an event without knowing who reacts to it.
struct ITaskObserver {
    virtual ~ITaskObserver() = default;
    virtual void onCompleted(const std::string& title) = 0;
};
class ProgressCounter final : public ITaskObserver {
public:
    void onCompleted(const std::string&) override {
        ++count;
    }
    int count = 0;
};
class ActivityFeed final : public ITaskObserver {
public:
    void onCompleted(const std::string& title) override {
        lastMessage = "Completed: " + title;
    }
    std::string lastMessage;
};
class TaskEvents {
public:
    void subscribe(ITaskObserver& observer) {
        observers_.push_back(observer);
    }
    void completed(const std::string& title) {
        for (ITaskObserver& observer : observers_) {
            observer.onCompleted(title);
        }
    }

private:
    // Borrowed, synchronous listeners. No unsubscribe or mutation during callbacks.
    std::vector<std::reference_wrapper<ITaskObserver>> observers_;
};
void run(const lab::Context&) {
    ProgressCounter counter;
    ActivityFeed feed; // Subscribers outlive the publisher.
    TaskEvents events;
    events.subscribe(counter);
    events.subscribe(feed);
    events.completed("Read chapter 1");
    lab::check(counter.count == 1 && feed.lastMessage == "Completed: Read chapter 1",
               "One event reaches two independent listeners");
    lab::lesson("Observer pattern: add reactions without adding dependencies to the publisher.");
}
} // namespace ex13
