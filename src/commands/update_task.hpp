#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class UpdateTask : public CommandCRTP<UpdateTask> {
public:
                            UpdateTask(TaskStorage& task_storage);

    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Update a task's description.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Update a task's description.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "update";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "update [task_id] [new_description]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::UpdateTask;
    }

private:
    TaskStorage& task_storage;
};

} // namespace cmds
