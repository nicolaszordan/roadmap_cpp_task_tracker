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
            std::println("id:{} - description:{}", task.id, task.description);
        }
    } else if (args.size() == 2) {
        if (args[1] == "done") {
            for (const auto& task: task_storage.view_all_tasks_done()) {
                std::println("id:{} - description:{}", task.id, task.description);
            }
        } else if (args[1] == "todo") {
            for (const auto& task: task_storage.view_all_tasks_todo()) {
                std::println("id:{} - description:{}", task.id, task.description);
            }
        } else if (args[1] == "in-progress") {
            for (const auto& task: task_storage.view_all_tasks_in_progress()) {
                std::println("id:{} - description:{}", task.id, task.description);
            }
        } else {
            return std::unexpected(CommandError::InvalidArguments);
        }
    } else {
        return std::unexpected(CommandError::InvalidArguments);
    }

    return {};
}

auto ListTasks::help() const -> std::string_view
{
    return ListTasks::get_help();
}

auto ListTasks::get_help() -> std::string_view
{
    return "ListTasks all tasks or tasks with a specific status.";
}

auto ListTasks::description() const -> std::string_view
{
    return ListTasks::get_description();
}

auto ListTasks::get_description() -> std::string_view
{
    return "ListTasks all tasks or tasks with a specific status.";
}

auto ListTasks::name() const -> std::string_view
{
    return ListTasks::get_name();
}

auto ListTasks::get_name() -> std::string_view
{
    return "list";
}

auto ListTasks::usage() const -> std::string_view
{
    return ListTasks::get_usage();
}

auto ListTasks::get_usage() -> std::string_view
{
    return "list [done|todo|in-progress]";
}

} // namespace cmds
