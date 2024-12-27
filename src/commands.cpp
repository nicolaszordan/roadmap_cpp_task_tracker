#include "commands.hpp"

#include <print>

auto cmds::command_type_from_string(std::string_view str) -> std::optional<CommandType>
{
    if (str == Echo::get_name()) {
        return CommandType::Echo;
    }

    return std::nullopt;
}

auto cmds::print_command_types() -> void
{
    std::println("Commands:");
    std::println("  echo [args..] - Prints the arguments to the screen.");
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

    SECTION("Unknown command")
    {
        auto result = cmds::command_type_from_string("unknown");
        REQUIRE(!result);
    }
}

} // namespace tests
