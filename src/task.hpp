#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include <format>
#include <chrono>
#include <iostream>

using TaskID = std::uint64_t;
using TimePoint = std::chrono::system_clock::time_point;

using json = nlohmann::json;

enum class TaskStatus {
    Todo,
    Done,
    InProgress,
};

struct Task {
    TaskID          id;
    TaskStatus      status;
    std::string     description;
    TimePoint       created_at;
    TimePoint       updated_at;
};

// use this struct to update a task
struct TaskUpdate {
    std::optional<std::string>  description = {};
    std::optional<TaskStatus>   status = {};
};

auto to_json(json& j, const TaskStatus& status) -> void;
auto from_json(const json& j, TaskStatus& status) -> void;

auto operator<<(std::ostream& os, const TaskStatus& status) -> std::ostream&;

template<>
struct std::formatter<TaskStatus, char>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext&& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const TaskStatus& task_status, FormatContext& ctx) const
    {
        switch (task_status) {
        case TaskStatus::Todo:
            return std::format_to(ctx.out(), "Todo");
        case TaskStatus::Done:
            return std::format_to(ctx.out(), "Done");
        case TaskStatus::InProgress:
            return std::format_to(ctx.out(), "InProgress");
        default:
            return std::format_to(ctx.out(), "Unknown");
        }
    }
};

auto operator==(const Task& lhs, const Task& rhs) -> bool;

auto to_json(json& j, const Task& task) -> void;
auto from_json(const json& j, Task& task) -> void;

auto operator<<(std::ostream& os, const Task& task) -> std::ostream&;

template<>
struct std::formatter<Task, char>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext&& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Task& task, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "Task(id:{}, status:{}, description:{}, created_at:{}, updated_at:{})"
            , task.id
            , task.status
            , task.description
            , task.created_at
            , task.updated_at
        );
    }
};
