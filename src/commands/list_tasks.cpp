#include "list_tasks.hpp"

#include <print>

#include "../task_storage.hpp"

namespace {

template<typename ViewT>
void print_tasks_in_view(ViewT&& task_view)
{
    for (const auto& task: task_view) {
        std::println("{}", task);
    }
}

} // anonymous namespace

namespace cmds {

ListTasks::ListTasks(TaskStorage& task_storage)
    : task_storage{ task_storage }
{
}

auto ListTasks::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    // list all tasks and return
    if (args.size() == 1) {
        print_tasks_in_view(task_storage.view_all_tasks());
        return {};
    } 
    
    // invalid number of arguments
    if (args.size() != 2) {
        return std::unexpected(CommandError::InvalidArguments);
    }

    const auto& filter = args[1];

    if (filter == "done") {
        print_tasks_in_view(task_storage.view_all_tasks_done());

    } else if (filter == "todo") {
        print_tasks_in_view(task_storage.view_all_tasks_todo());

    } else if (filter == "in-progress") {
        print_tasks_in_view(task_storage.view_all_tasks_in_progress());

    } else {
        return std::unexpected(CommandError::InvalidArguments);
    }

    return {};
}

} // namespace cmds
