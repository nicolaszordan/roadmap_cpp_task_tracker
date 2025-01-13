#include "commands.hpp"

#include <print>

auto cmds::command_type_from_string(std::string_view str) -> std::optional<CommandType>
{
    if (str == Echo::get_name()) {
        return CommandType::Echo;
    } else if (str == ListTasks::get_name()) {
        return CommandType::ListTasks;
    } else if (str == AddTask::get_name()) {
        return CommandType::AddTask;
    } else if (str == DeleteTask::get_name()) {
        return CommandType::DeleteTask;
    } else if (str == MarkTaskDone::get_name()) {
        return CommandType::MarkTaskDone;
    } else if (str == MarkTaskInProgress::get_name()) {
        return CommandType::MarkTaskInProgress;
    } else if (str == UpdateTask::get_name()) {
        return CommandType::UpdateTask;
    } else if (str == Help::get_name()) {
        return CommandType::Help;
    }

    return std::nullopt;
}

auto cmds::print_command_types() -> void
{
    std::println("Commands:");
    std::println("  {} - {}", Help::get_name(), Help::get_description());
    std::println("  {} - {}", Echo::get_name(), Echo::get_description());
    std::println("  {} - {}", ListTasks::get_name(), ListTasks::get_description());
    std::println("  {} - {}", AddTask::get_name(), AddTask::get_description());
    std::println("  {} - {}", DeleteTask::get_name(), DeleteTask::get_description());
    std::println("  {} - {}", MarkTaskDone::get_name(), MarkTaskDone::get_description());
    std::println("  {} - {}", MarkTaskInProgress::get_name(), MarkTaskInProgress::get_description());
    std::println("  {} - {}", UpdateTask::get_name(), UpdateTask::get_description());
}

#ifdef UNIT_TESTS

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

    SECTION("AddTask")
    {
        auto result = cmds::command_type_from_string("add");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::AddTask);
    }

    SECTION("DeleteTask")
    {
        auto result = cmds::command_type_from_string("delete");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::DeleteTask);
    }

    SECTION("MarkTaskDone")
    {
        auto result = cmds::command_type_from_string("mark-task-done");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::MarkTaskDone);
    }

    SECTION("MarkTaskInProgress")
    {
        auto result = cmds::command_type_from_string("mark-task-in-progress");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::MarkTaskInProgress);
    }

    SECTION("UpdateTask")
    {
        auto result = cmds::command_type_from_string("update");
        REQUIRE(result);
        REQUIRE(*result == cmds::CommandType::UpdateTask);
    }

    SECTION("Unknown command")
    {
        auto result = cmds::command_type_from_string("unknown");
        REQUIRE(!result);
    }
}

} // namespace tests

#endif // UNIT_TESTS
