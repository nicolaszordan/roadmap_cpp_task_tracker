#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class DeleteTask : public CommandCRTP<DeleteTask> {
public:
                            DeleteTask(TaskStorage& task_storage);

    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Delete a task.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Delete a task.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "delete";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "delete [task_id]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::DeleteTask;
    }

private:
    TaskStorage& task_storage;
};

} // namespace cmds
