#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <expected>

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
