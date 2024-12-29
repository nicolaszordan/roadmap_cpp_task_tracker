#include "echo.hpp"

#include <print>

namespace cmds {

auto Echo::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    for (const auto& arg : args) {
        std::println("{}", arg);
    }
}

auto Echo::help() const -> std::string_view
{
    return Echo::get_help();
}

auto Echo::get_help() -> std::string_view
{
    return "Prints the arguments to the screen.";
}

auto Echo::description() const -> std::string_view
{
    return Echo::get_description();
}

auto Echo::get_description() -> std::string_view
{
    return "Prints the arguments to the screen.";
}

auto Echo::name() const -> std::string_view
{
    return Echo::get_name();
}

auto Echo::get_name() -> std::string_view
{
    return "echo";
}

auto Echo::usage() const -> std::string_view
{
    return Echo::get_usage();
}

auto Echo::get_usage() -> std::string_view
{
    return "echo [args..]";
}

} // namespace cmds
