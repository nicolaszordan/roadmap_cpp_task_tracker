#include "list_tasks.hpp"

#include <print>

#include "../task_storage.hpp"

namespace cmds {

ListTasks::ListTasks(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto ListTasks::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() == 1) {
        for (const auto& task: task_storage.view_all_tasks()) {
            std::println("{}", task);
        }

    } else if (args.size() == 2) {
        if (args[1] == "done") {
            for (const auto& task: task_storage.view_all_tasks_done()) {
                std::println("{}", task);
            }

        } else if (args[1] == "todo") {
            for (const auto& task: task_storage.view_all_tasks_todo()) {
                std::println("{}", task);
            }

        } else if (args[1] == "in-progress") {
            for (const auto& task: task_storage.view_all_tasks_in_progress()) {
                std::println("{}", task);
            }

        } else {
            return std::unexpected(CommandError::InvalidArguments);
        }

    } else {
        return std::unexpected(CommandError::InvalidArguments);
    }

    return {};
}

} // namespace cmds
