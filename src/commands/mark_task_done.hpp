#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class MarkTaskDone : public CommandCRTP<MarkTaskDone> {
public:
                            MarkTaskDone(TaskStorage& task_storage);

    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Mark a task as Done.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Mark a task as Done.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "mark-done";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "mark-done [task_id]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::MarkTaskDone;
    }

private:
    TaskStorage& task_storage;
};

} // namespace cmds
