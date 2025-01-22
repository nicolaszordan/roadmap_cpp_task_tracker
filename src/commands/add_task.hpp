#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class AddTask : public CommandCRTP<AddTask> {
public:
                            AddTask(TaskStorage& task_storage);

    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Add a new task.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Add a new task.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "add";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "add [description]";
    }

   constexpr static auto   get_type() -> CommandType
    {
        return CommandType::AddTask;
    } 

private:
    TaskStorage& task_storage;
};

} // namespace cmds
