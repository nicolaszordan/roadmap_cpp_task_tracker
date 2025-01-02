#include "add_task.hpp"

#include <print>

#include "../task_storage.hpp"
#include "../task.hpp"

namespace cmds {

AddTask::AddTask(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto AddTask::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() != 2) {
        return std::unexpected(CommandError::InvalidArguments);
    }

    auto description = args[1];

    auto task_id = task_storage.add_task(description);

    std::println("Task added: id:{} - description:{}", task_id, description);

    return {};
}

auto AddTask::help() const -> std::string_view
{
    return AddTask::get_help();
}

auto AddTask::get_help() -> std::string_view
{
    return "Add a new task.";
}

auto AddTask::description() const -> std::string_view
{
    return AddTask::get_description();
}

auto AddTask::get_description() -> std::string_view
{
    return "Add a new task.";
}

auto AddTask::name() const -> std::string_view
{
    return AddTask::get_name();
}

auto AddTask::get_name() -> std::string_view
{
    return "add";
}

auto AddTask::usage() const -> std::string_view
{
    return AddTask::get_usage();
}

auto AddTask::get_usage() -> std::string_view
{
    return "add [description]";
}

} // namespace cmds