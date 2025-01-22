#include "mark_task_done.hpp"

#include <print>

#include "../task_storage.hpp"
#include "../task.hpp"

namespace cmds {

MarkTaskDone::MarkTaskDone(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto MarkTaskDone::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() != 2) {
        return std::unexpected(CommandError::InvalidArguments);
    }

    auto task_id = std::stoul(args[1]);

    auto task_update = TaskUpdate{ status: TaskStatus::Done };

    auto updated_task = task_storage.update_task(task_id, task_update);

    if (!updated_task) {
        return std::unexpected(CommandError::InvalidTaskID);
    }

    std::println("Task updated: id:{} - status:{}", updated_task->get().id, "done");

    return {};
}

} // namespace cmds
