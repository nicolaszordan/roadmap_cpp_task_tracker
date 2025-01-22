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

    if (!updated_task.has_value()) {
        return std::unexpected(CommandError::InvalidTaskID);
    }

    std::println("Task updated: id:{} - status:{}", updated_task->get().id, "in-progress");

    return {};
}

} // namespace cmds
