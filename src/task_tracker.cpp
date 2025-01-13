#include "task_tracker.hpp"

#include <print>
#include <iostream>
#include <string>

#include "commands.hpp"

namespace {

auto prompt_user() -> std::string
{
    std::string input_buffer;
    std::print("> ");
    std::getline(std::cin >> std::ws, input_buffer);
    return input_buffer;
}

auto should_quit(std::string_view input) -> bool
{
    return std::cin.eof() || input == "exit" || input == "quit";
}

auto quoted_split(std::string_view str, char delimiter) -> std::vector<std::string>
{
    std::vector<std::string> result;

    while (!str.empty()) {
        if (str.front() == '"') {
            // Skip the opening quote
            str.remove_prefix(1);
            auto pos = str.find('"');
            result.push_back(std::string{str.substr(0, pos)});
            // Fastforward to the end of the word and quote
            str.remove_prefix(pos + 1);

        } else if (str.front() == delimiter) {
            // Skip the delimiter
            str.remove_prefix(1);

        } else {
            auto pos = str.find(delimiter); // should also check for quotes
            result.push_back(std::string{str.substr(0, pos)});
            str.remove_prefix(pos != str.npos ? pos + 1 : str.size());
        }
    }

    return result;
}

} // anonymous namespace

TaskTracker::TaskTracker(std::optional<std::filesystem::path> file_storage)
    : task_storage{ file_storage }
{
}

auto TaskTracker::interactive_mode() -> void
{
    for (
        auto input_buffer = prompt_user();
        !should_quit(input_buffer);
        input_buffer = prompt_user()
    ) {
        if (input_buffer.empty()) {
            continue;
        }

        auto args = quoted_split(input_buffer, ' ');

        batch_mode(args);
    }
}

auto TaskTracker::batch_mode(const std::vector<std::string>& args) -> void
{
    if (args.empty()) {
        std::println("No command provided.");
        return;
    }

    auto command_name = args.front();
    auto command = create_command(command_name);
    if (!command) {
        std::println("Unknown command: {}", command_name);
        cmds::print_command_types();
        return;
    }

    auto run_result = command->run(args);
    
    if (run_result.error() == cmds::CommandError::InvalidArguments) {
        std::println("Invalid arguments for command: {}", command_name);
        std::println("Usage: {}", command->usage());
        return;
    } else if (!run_result) {
        std::println("Error running command: {}", run_result.error());
        return;
    }

    task_storage.save_tasks();
}

// NOTE: this could return a variant instead of a unique_ptr to avoid an allocation
auto TaskTracker::create_command(std::string_view command_name) -> std::unique_ptr<cmds::Command>
{
    auto command_type = cmds::command_type_from_string(command_name);
    if (!command_type) {
        return nullptr;
    }

    switch (*command_type) {
    case cmds::CommandType::Echo:
        return std::make_unique<cmds::Echo>();

    case cmds::CommandType::Help:
        return std::make_unique<cmds::Help>();

    case cmds::CommandType::ListTasks:
        return std::make_unique<cmds::ListTasks>(task_storage);

    case cmds::CommandType::AddTask:
        return std::make_unique<cmds::AddTask>(task_storage);

    case cmds::CommandType::DeleteTask:
        return std::make_unique<cmds::DeleteTask>(task_storage);

    case cmds::CommandType::UpdateTask:
        return std::make_unique<cmds::UpdateTask>(task_storage);

    case cmds::CommandType::MarkTaskDone:
        return std::make_unique<cmds::MarkTaskDone>(task_storage);

    case cmds::CommandType::MarkTaskInProgress:
        return std::make_unique<cmds::MarkTaskInProgress>(task_storage);
    
    default:
        return nullptr;
    }
}


#ifdef UNIT_TESTS

#include <catch2/catch_test_macros.hpp>
namespace tests {

TEST_CASE("quoted_split can split strings into words", "[quoted_split]")
{
    auto result = quoted_split("echo \"Hello, World!\"", ' ');

    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == "echo");
    REQUIRE(result[1] == "Hello, World!");
}

TEST_CASE("quoted_split can split strings into words with multiple spaces", "[quoted_split]")
{
    auto result = quoted_split("echo   \"Hello, World!\"", ' ');

    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == "echo");
    REQUIRE(result[1] == "Hello, World!");
}

TEST_CASE("quoted_split can split strings into words with multiple quoted strings", "[quoted_split]")
{
    auto result = quoted_split("echo \"Hello, World!\" \"Goodbye, World!\"", ' ');

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "echo");
    REQUIRE(result[1] == "Hello, World!");
    REQUIRE(result[2] == "Goodbye, World!");
}

TEST_CASE("quoted_split can split strings into words with multiple quoted strings and spaces", "[quoted_split]")
{
    auto result = quoted_split("echo   \"Hello, World!\"   \"Goodbye, World!\"", ' ');

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == "echo");
    REQUIRE(result[1] == "Hello, World!");
    REQUIRE(result[2] == "Goodbye, World!");
}

} // namespace tests

#endif // UNIT_TESTS