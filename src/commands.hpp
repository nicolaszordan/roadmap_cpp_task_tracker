#pragma once

#include <string_view>
#include <optional>

#include "commands/command.hpp"
#include "commands/echo.hpp"
#include "commands/list_tasks.hpp"
#include "commands/add_task.hpp"
#include "commands/delete_task.hpp"
#include "commands/update_task.hpp"
#include "commands/mark_task_done.hpp"
#include "commands/mark_task_in_progress.hpp"

namespace cmds {

enum class CommandType {
    Echo,
    ListTasks,
    AddTask,
    DeleteTask,
    UpdateTask,
    MarkTaskDone,
    MarkTaskInProgress,
};

auto    command_type_from_string(std::string_view str) -> std::optional<CommandType>;
auto    print_command_types() -> void;

} // namespace cmds
