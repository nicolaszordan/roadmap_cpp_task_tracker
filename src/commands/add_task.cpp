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

} // namespace cmds