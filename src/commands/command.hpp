#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <expected>
#include <format>

namespace cmds {

enum class CommandError {
    UnknownCommand,
    InvalidArguments,
    InvalidTaskID,
};

class Command {
public:
    virtual         ~Command() = default;

    // Run the command with the given arguments.
    virtual auto    run(const std::vector<std::string>& args) -> std::expected<void, CommandError> = 0;

    // Provide a help message for the command.
    virtual auto    help() const -> std::string_view = 0;

    // Provide a short description of the command.
    virtual auto    description() const -> std::string_view = 0;

    // Provide a name for the command.
    virtual auto    name() const -> std::string_view = 0;

    // Provide a usage message for the command.
    virtual auto    usage() const -> std::string_view = 0;
};
    
} // namespace cmds

template<>
struct std::formatter<cmds::CommandError, char>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext&& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const cmds::CommandError& error, FormatContext& ctx) const
    {
        switch (error) {
        case cmds::CommandError::UnknownCommand:
            return std::format_to(ctx.out(), "UnknownCommand");
        case cmds::CommandError::InvalidArguments:
            return std::format_to(ctx.out(), "InvalidArguments");
        case cmds::CommandError::InvalidTaskID:
            return std::format_to(ctx.out(), "InvalidTaskID");
        default:
            return std::format_to(ctx.out(), "UnknownError");
        }
    }
};