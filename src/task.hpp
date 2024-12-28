#pragma once

#include <cstdint>
#include <string>

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
