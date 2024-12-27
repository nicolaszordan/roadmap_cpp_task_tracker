#pragma once

#include "command.hpp"

namespace cmds {

class Echo : public Command {
public:
    auto        run(const std::vector<std::string>& args) -> void override;

    auto        help() const -> std::string_view override;
    static auto get_help() -> std::string_view;

    auto        description() const -> std::string_view override;
    static auto get_description() -> std::string_view;

    auto        name() const -> std::string_view override;
    static auto get_name() -> std::string_view;

    auto        usage() const -> std::string_view override;
    static auto get_usage() -> std::string_view;
};

} // namespace cmds