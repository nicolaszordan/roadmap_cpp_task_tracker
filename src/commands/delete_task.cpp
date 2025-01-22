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

} // namespace cmds