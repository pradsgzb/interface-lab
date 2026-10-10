#include "studydesk/core.hpp"
#include "studydesk/example.hpp"
#include <algorithm>
#include <iomanip>

namespace {
void usage() {
    std::cout << "Interface Lab: 21 lessons in practical C++ interfaces\n"
              << "  --list                  List lessons\n"
              << "  --example <1-21>        Run one lesson\n"
              << "  --all                   Run every lesson and its checks\n"
              << "  --cui                   Interactive StudyDesk console\n"
              << "  --plugin <file>         Plugin for lesson 1\n"
              << "  --output <directory>    Demo output (default: lab-output)\n";
}
} // namespace
int main(int argc, char* argv[]) {
    try {
        lab::Context context{
            std::filesystem::absolute(argv[0]).parent_path() / STUDY_PLUGIN_FILENAME, "lab-output"};
        std::string mode;
        int selected = 0;
        for (int i = 1; i < argc; ++i) {
            const std::string argument = argv[i];
            if (argument == "--help" || argument == "-h") {
                usage();
                return 0;
            }
            if (argument == "--plugin" || argument == "--output") {
                if (++i == argc) {
                    throw std::invalid_argument("Missing path after " + argument);
                }
                if (argument == "--plugin") {
                    context.pluginPath = argv[i];
                } else {
                    context.outputDirectory = argv[i];
                }
            } else if (argument == "--example" || argument == "--all" || argument == "--list" ||
                       argument == "--cui") {
                if (!mode.empty()) {
                    throw std::invalid_argument("Choose one run mode");
                }
                mode = argument;
                if (mode == "--example") {
                    if (++i == argc) {
                        throw std::invalid_argument("Missing example number");
                    }
                    const std::string number = argv[i];
                    std::size_t consumed = 0;
                    selected = std::stoi(number, &consumed);
                    if (consumed != number.size() || selected < 1 || selected > 21) {
                        throw std::invalid_argument("Example must be 1-21");
                    }
                }
            } else {
                throw std::invalid_argument("Unknown option: " + argument);
            }
        }
        if (mode.empty()) {
            usage();
            return 0;
        }
        if (mode == "--list") {
            for (const auto& example : lab::examples()) {
                std::cout << std::setw(2) << example.id << "  " << example.title << '\n';
            }
            return 0;
        }
        if (mode == "--cui") {
            studydesk::MemoryTaskStore store;
            studydesk::StreamLogger logger(std::cout);
            studydesk::TaskService service(store, logger);
            studydesk::ConsoleFrontend console(std::cin, std::cout);
            return console.run(service);
        }
        std::filesystem::create_directories(context.outputDirectory);
        for (const auto& example : lab::examples()) {
            if (mode == "--all" || example.id == selected) {
                std::cout << "\n[" << example.id << "] " << example.title << '\n';
                example.run(context);
            }
        }
        std::cout << "\nAll selected lesson checks passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
