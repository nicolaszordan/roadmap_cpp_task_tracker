#pragma once

#include "command.hpp"

namespace cmds {

class Help : public CommandCRTP<Help> {
public:
    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Provides help and informations for commands.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Provides help and informations for commands.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "help";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "help [command]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::Help;
    }
};

} // namespace cmds
