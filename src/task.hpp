#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

using TaskID = std::uint64_t;

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
    // TODO:        created_at
    // TODO:        updated_at
};

auto operator==(const Task& lhs, const Task& rhs) -> bool;

// use this struct to update a task
struct TaskUpdate {
    std::optional<std::string>  description = {};
    std::optional<TaskStatus>   status = {};
};

auto to_json(json& j, const TaskStatus& status) -> void;
auto from_json(const json& j, TaskStatus& status) -> void;

auto to_json(json& j, const Task& task) -> void;
auto from_json(const json& j, Task& task) -> void;