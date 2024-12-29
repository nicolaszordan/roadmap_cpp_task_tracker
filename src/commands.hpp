#pragma once

#include <string_view>
#include <optional>

#include "commands/command.hpp"
#include "commands/echo.hpp"
#include "commands/list_tasks.hpp"

namespace cmds {

enum class CommandType {
    Echo,
    ListTasks,
};

auto    command_type_from_string(std::string_view str) -> std::optional<CommandType>;
auto    print_command_types() -> void;

} // namespace cmds
