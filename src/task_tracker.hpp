#pragma once

#include <string>
#include <vector>
#include <memory>

#include "commands/command.hpp"

class TaskTracker {
public:
    auto        interactive_mode() -> void;
    auto        batch_mode(const std::vector<std::string>& args) -> void;

private:
    auto        create_command(std::string_view command_name) -> std::unique_ptr<cmds::Command>;
};
