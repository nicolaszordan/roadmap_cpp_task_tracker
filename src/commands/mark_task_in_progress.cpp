#include "mark_task_in_progress.hpp"

#include <print>

#include "../task_storage.hpp"
#include "../task.hpp"

namespace cmds {

MarkTaskInProgress::MarkTaskInProgress(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto MarkTaskInProgress::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() != 2) {
        return std::unexpected(CommandError::InvalidArguments);
    }

    auto task_id = std::stoul(args[1]);

    auto task_update = TaskUpdate{ status: TaskStatus::InProgress };

    auto updated_task = task_storage.update_task(task_id, task_update);

    if (!updated_task) {
        return std::unexpected(CommandError::InvalidTaskID);
    }

    std::println("Task updated: id:{} - status:{}", updated_task->get().id, "in-progress");

    return {};
}

auto MarkTaskInProgress::help() const -> std::string_view
{
    return MarkTaskInProgress::get_help();
}

auto MarkTaskInProgress::get_help() -> std::string_view
{
    return "Mark a task as InProgress.";
}

auto MarkTaskInProgress::description() const -> std::string_view
{
    return MarkTaskInProgress::get_description();
}

auto MarkTaskInProgress::get_description() -> std::string_view
{
    return "Mark a task as InProgress.";
}

auto MarkTaskInProgress::name() const -> std::string_view
{
    return MarkTaskInProgress::get_name();
}

auto MarkTaskInProgress::get_name() -> std::string_view
{
    return "mark-task-in-progress";
}

auto MarkTaskInProgress::usage() const -> std::string_view
{
    return MarkTaskInProgress::get_usage();
}

auto MarkTaskInProgress::get_usage() -> std::string_view
{
    return "mark-in-progress [task_id]";
}

} // namespace cmds
