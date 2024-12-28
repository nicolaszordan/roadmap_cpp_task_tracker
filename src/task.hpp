#pragma once

#include <cstdint>
#include <string>
#include <optional>

using TaskID = std::uint64_t;

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

// use this struct to update a task
struct TaskUpdate {
    std::optional<std::string>  description = {};
    std::optional<TaskStatus>   status = {};
};

