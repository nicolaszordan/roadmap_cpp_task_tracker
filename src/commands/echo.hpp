#pragma once

#include "command.hpp"

namespace cmds {

class Echo : public CommandCRTP<Echo> {
public:
    auto                    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> override;

    constexpr static auto   get_help() -> std::string_view
    {
        return "Prints the arguments to the screen.";
    }

    constexpr static auto   get_description() -> std::string_view
    {
        return "Prints the arguments to the screen.";
    }

    constexpr static auto   get_name() -> std::string_view
    {
        return "echo";
    }

    constexpr static auto   get_usage() -> std::string_view
    {
        return "echo [args..]";
    }

    constexpr static auto   get_type() -> CommandType
    {
        return CommandType::Echo;
    }
};

} // namespace cmds