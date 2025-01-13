#include "update_task.hpp"

#include <print>

#include "../task_storage.hpp"
#include "../task.hpp"

namespace cmds {

UpdateTask::UpdateTask(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto UpdateTask::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() != 3) {
        return std::unexpected(CommandError::InvalidArguments);
    }

    auto task_id = std::stoul(args[1]);
    auto new_description = args[2];

    auto task_update = TaskUpdate{ description: new_description };

    auto updated_task = task_storage.update_task(task_id, task_update);

    if (!updated_task) {
        return std::unexpected(CommandError::InvalidTaskID);
    }

    std::println("Task updated: id:{} - description:{}", updated_task->get().id, updated_task->get().description);

    return {};
}

auto UpdateTask::help() const -> std::string_view
{
    return UpdateTask::get_help();
}

auto UpdateTask::get_help() -> std::string_view
{
    return "UpdateTask all tasks or tasks with a specific status.";
}

auto UpdateTask::description() const -> std::string_view
{
    return UpdateTask::get_description();
}

auto UpdateTask::get_description() -> std::string_view
{
    return "UpdateTask all tasks or tasks with a specific status.";
}

auto UpdateTask::name() const -> std::string_view
{
    return UpdateTask::get_name();
}

auto UpdateTask::get_name() -> std::string_view
{
    return "update";
}

auto UpdateTask::usage() const -> std::string_view
{
    return UpdateTask::get_usage();
}

auto UpdateTask::get_usage() -> std::string_view
{
    return "update [task_id] [new_description]";
}

} // namespace cmds
