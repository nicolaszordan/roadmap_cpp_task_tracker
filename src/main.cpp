#include <vector>
#include <string>
#include <print>

#include "task_tracker.hpp"

auto interactive_mode() -> void
{
    TaskTracker task_tracker;

    task_tracker.interactive_mode();
}

auto batch_mode(const std::vector<std::string>& args) -> void
{
    TaskTracker task_tracker;

    task_tracker.batch_mode(args);
}

auto print_usage() -> void
{
    std::println("Usage: task_tracker [command] [args...]");
    std::println("  running with no arguments will start interactive mode.");
    std::println("");
    std::println("Arguments:");
    std::println("  command: The command to execute.");
    std::println("  args: The arguments to pass to the command.");
    std::println("Commands:");
}

int main(int argc, char* const argv[])
{
    if (argc == 1) {
        // start in interactive mode
        interactive_mode();
    } else {
        // start in batch mode
        std::vector<std::string> args(argv + 1, argv + argc);
        batch_mode(args);
    }
    return 0;
}
