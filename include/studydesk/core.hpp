#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace studydesk {

struct Task {
    std::string title;
};

// A contract says WHAT a caller may request, not HOW it will be done.
// A virtual destructor makes destruction through an interface pointer safe.
struct ILogger {
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

struct ITaskStore {
    virtual ~ITaskStore() = default;
    virtual void add(const Task& task) = 0;
    virtual std::vector<Task> all() const = 0;
};

struct ITaskService {
    virtual ~ITaskService() = default;
    // Contract: reject blank / >80-byte / control-character titles with
    // std::invalid_argument; otherwise add exactly one task. Return a snapshot.
    virtual void addTask(const std::string& title) = 0;
    virtual std::vector<Task> tasks() const = 0;
};

struct IFrontend {
    virtual ~IFrontend() = default;
    // Each implementation owns its interaction loop. The service outlives it.
    virtual int run(ITaskService& service) = 0;
};

class MemoryTaskStore final : public ITaskStore {
public:
    void add(const Task& task) override;
    std::vector<Task> all() const override;

private:
    std::vector<Task> tasks_;
};

class StreamLogger final : public ILogger {
public:
    explicit StreamLogger(std::ostream& output) : output_(output) {}
    void log(const std::string& message) override;

private:
    std::ostream& output_; // Borrowed: caller owns the stream.
};

// Constructor injection: core logic depends on contracts, not a UI or database.
class TaskService final : public ITaskService {
public:
    TaskService(ITaskStore& store, ILogger& logger) : store_(store), logger_(logger) {}
    void addTask(const std::string& title) override;
    std::vector<Task> tasks() const override;

private:
    ITaskStore& store_; // Borrowed dependencies must outlive this object.
    ILogger& logger_;
};

class ConsoleFrontend final : public IFrontend {
public:
    ConsoleFrontend(std::istream& input, std::ostream& output) : input_(input), output_(output) {}
    int run(ITaskService& service) override;

private:
    std::istream& input_;
    std::ostream& output_;
};

} // namespace studydesk
