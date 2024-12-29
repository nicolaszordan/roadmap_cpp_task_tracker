#include "commands.hpp"

#include <print>

auto cmds::command_type_from_string(std::string_view str) -> std::optional<CommandType>
{
    if (str == Echo::get_name()) {
        return CommandType::Echo;
    } else if (str == ListTasks::get_name()) {
        return CommandType::ListTasks;
    }

    return std::nullopt;
}

auto cmds::print_command_types() -> void
{
    std::println("Commands:");
    std::println("  {} - {}", Echo::get_name(), Echo::get_description());
    std::println("  {} - {}", ListTasks::get_name(), ListTasks::get_description());
}

#include <catch2/catch_test_macros.hpp>
namespace tests {

TEST_CASE("command_type_from_string returns the correct command type", "[commands]")
{
    SECTION("Echo")
    {
        auto result = cmds::command_type_from_string("echo");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::Echo);
    }
    
    SECTION("ListTasks")
    {
        auto result = cmds::command_type_from_string("list");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::ListTasks);
    }

    SECTION("Unknown command")
    {
        auto result = cmds::command_type_from_string("unknown");
        REQUIRE(!result);
    }
}

} // namespace tests
