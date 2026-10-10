#pragma once
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace lab {
struct Context {
    std::filesystem::path pluginPath;
    std::filesystem::path outputDirectory;
};
struct Example {
    int id;
    const char* title;
    void (*run)(const Context&);
};
const std::vector<Example>& examples();
// Unlike assert(), this check remains active in Release builds.
inline void check(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("Check failed: " + message);
    }
    std::cout << "  PASS: " << message << '\n';
}
inline void lesson(const char* text) {
    std::cout << "  WHY: " << text << '\n';
}
} // namespace lab
