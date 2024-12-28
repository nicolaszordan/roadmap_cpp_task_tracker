#pragma once

#include <string_view>
#include <unordered_map>
#include <optional>
#include <functional>
#include <ranges>

#include "task.hpp"

class TaskStorage {
public:
            TaskStorage() = default;
            ~TaskStorage() = default;

public:
    auto    add_task(std::string_view description) -> TaskID;
    auto    get_task(TaskID id) const -> std::optional<std::reference_wrapper<const Task>>;
    auto    update_task(TaskID id, const TaskUpdate& update) -> std::optional<std::reference_wrapper<const Task>>;
    auto    delete_task(TaskID id) -> void;

    auto    get_tasks_map() const -> const std::unordered_map<TaskID, Task>&;

    // views
    auto    view_all_tasks() const -> auto {
        return tasks | std::views::values;
    }

    auto    view_all_tasks_todo() -> auto {
        return tasks | std::views::values | std::views::filter([](const Task& task) {
            return task.status == TaskStatus::Todo;
        });
    }

    auto    view_all_tasks_in_progress() -> auto {
        return tasks | std::views::values | std::views::filter([](const Task& task) {
            return task.status == TaskStatus::InProgress;
        });
    }

    auto    view_all_tasks_done() -> auto {
        return tasks | std::views::values | std::views::filter([](const Task& task) {
            return task.status == TaskStatus::Done;
        });
    }

private:
    TaskID                              next_id = {};
    std::unordered_map<TaskID, Task>    tasks;
};
