#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class MarkTaskInProgress : public CommandCRTP<MarkTaskInProgress> {
public:
                            MarkTaskInProgress(TaskStorage& task_storage);

    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Mark a task as InProgress.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Mark a task as InProgress.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "mark-in-progress";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "mark-in-progress [task_id]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::MarkTaskInProgress;
    }

private:
    TaskStorage& task_storage;
};

} // namespace cmds
