#include "delete_task.hpp"

#include <print>

#include "../task_storage.hpp"

namespace cmds {

DeleteTask::DeleteTask(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto DeleteTask::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() != 2) {
        return std::unexpected(CommandError::InvalidArguments);
    }

    auto task_id = std::stoul(args[1]);

    task_storage.delete_task(task_id);

    std::println("Task deleted: id:{}", task_id);

    return {};
}

auto DeleteTask::help() const -> std::string_view
{
    return DeleteTask::get_help();
}

auto DeleteTask::get_help() -> std::string_view
{
    return "Delete a task.";
}

auto DeleteTask::description() const -> std::string_view
{
    return DeleteTask::get_description();
}

auto DeleteTask::get_description() -> std::string_view
{
    return "Delete a task.";
}

auto DeleteTask::name() const -> std::string_view
{
    return DeleteTask::get_name();
}

auto DeleteTask::get_name() -> std::string_view
{
    return "delete";
}

auto DeleteTask::usage() const -> std::string_view
{
    return DeleteTask::get_usage();
}

auto DeleteTask::get_usage() -> std::string_view
{
    return "delete [task_id]";
}

} // namespace cmds