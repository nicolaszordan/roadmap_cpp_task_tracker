#include "task_tracker.hpp"

#include <print>

#include "commands.hpp"

auto TaskTracker::interactive_mode() -> void
{
    std::println("Interactive mode not yet implemented.");
}

auto TaskTracker::batch_mode(const std::vector<std::string>& args) -> void
{
    if (args.empty()) {
        std::println("No command provided.");
        return;
    }

    auto command_name = args.front();
    auto command = create_command(command_name);
    if (!command) {
        std::println("Unknown command: {}", command_name);
        cmds::print_command_types();
        return;
    }

    command->run(args);
}

auto TaskTracker::create_command(std::string_view command_name) -> std::unique_ptr<cmds::Command>
{
    auto command_type = cmds::command_type_from_string(command_name);
    if (!command_type) {
        return nullptr;
    }

    switch (*command_type) {
    case cmds::CommandType::Echo:
        return std::make_unique<cmds::Echo>();
    
    default:
        return nullptr;
    }
}
