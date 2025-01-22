#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class ListTasks : public CommandCRTP<ListTasks> {
public:
                            ListTasks(TaskStorage& task_storage);

    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "List all tasks or tasks with a specific status.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "List all tasks or tasks with a specific status.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "list";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "list [todo|in-progress|done]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::ListTasks;
    }

private:
    TaskStorage& task_storage;
};

} // namespace cmds
