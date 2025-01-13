#include "help.hpp"

#include "../commands.hpp"

#include <print>

namespace {

template<typename CommandT>
void print_command_help_and_usage()
{
    std::println("{}", CommandT::get_help());
    std::println("{}", CommandT::get_usage());
}

} // anonymous namespace

namespace cmds {

auto Help::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    if (args.size() == 1) {
        print_command_types();
        return {};
    }

    auto command = cmds::command_type_from_string(args[1]);

    if (!command) {
        return std::unexpected(CommandError::UnknownCommand);
    }

    switch (*command) {
    case CommandType::Echo:
        print_command_help_and_usage<Echo>();
        return {};

    case CommandType::Help:
        print_command_help_and_usage<Help>();
        return {};

    case CommandType::ListTasks:
        print_command_help_and_usage<ListTasks>();
        return {};

    case CommandType::AddTask:
        print_command_help_and_usage<AddTask>();
        return {};

    case CommandType::DeleteTask:
        print_command_help_and_usage<DeleteTask>();
        return {};

    case CommandType::UpdateTask:
        print_command_help_and_usage<UpdateTask>();
        return {};

    case CommandType::MarkTaskDone:
        print_command_help_and_usage<MarkTaskDone>();
        return {};

    case CommandType::MarkTaskInProgress:
        print_command_help_and_usage<MarkTaskInProgress>();
        return {};

    default:
        return std::unexpected(CommandError::UnknownCommand);
    }
}

auto Help::help() const -> std::string_view
{
    return Help::get_help();
}

auto Help::get_help() -> std::string_view
{
    return "Provides help and informations for commands.";
}

auto Help::description() const -> std::string_view
{
    return Help::get_description();
}

auto Help::get_description() -> std::string_view
{
    return "Provides help and informations for commands.";
}

auto Help::name() const -> std::string_view
{
    return Help::get_name();
}

auto Help::get_name() -> std::string_view
{
    return "help";
}

auto Help::usage() const -> std::string_view
{
    return Help::get_usage();
}

auto Help::get_usage() -> std::string_view
{
    return "help [command]";
}

} // namespace cmds

