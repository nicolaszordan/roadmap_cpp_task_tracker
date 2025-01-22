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

} // namespace cmds
