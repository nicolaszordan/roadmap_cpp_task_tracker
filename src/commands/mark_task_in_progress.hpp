#pragma once

#include "command.hpp"

class TaskStorage;

namespace cmds {

class MarkTaskInProgress : public Command {
public:
                MarkTaskInProgress(TaskStorage& task_storage);

    auto        run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    auto        help() const -> std::string_view override;
    static auto get_help() -> std::string_view;

    auto        description() const -> std::string_view override;
    static auto get_description() -> std::string_view;

    auto        name() const -> std::string_view override;
    static auto get_name() -> std::string_view;

    auto        usage() const -> std::string_view override;
    static auto get_usage() -> std::string_view;

private:
    TaskStorage& task_storage;
};

} // namespace cmds
