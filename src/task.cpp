#include "task.hpp"

auto to_json(json& j, const TaskStatus& status) -> void
{
    switch (status) {
    case TaskStatus::Todo:
        j = "Todo";
        break;
    case TaskStatus::Done:
        j = "Done";
        break;
    case TaskStatus::InProgress:
        j = "InProgress";
        break;
    default:
        throw std::invalid_argument("Invalid TaskStatus");
        break;
    }
}

auto from_json(const json& j, TaskStatus& status) -> void
{
    auto status_str = j.get<std::string>();

    if (status_str == "Todo" || status_str == "todo") {
        status = TaskStatus::Todo;
    } else if (status_str == "Done" || status_str == "done") {
        status = TaskStatus::Done;
    } else if (status_str == "InProgress" 
        || status_str == "in-progress" 
        || status_str == "inprogress" 
        || status_str == "in progress" 
        || status_str == "in_progress"
    ) {
        status = TaskStatus::InProgress;
    } else {
        throw std::invalid_argument("Invalid TaskStatus");
    }
}

auto to_json(json& j, const Task& task) -> void
{
    j = json{
        {"id", task.id},
        {"status", task.status},
        {"description", task.description},
    };
}

auto from_json(const json& j, Task& task) -> void
{
    j.at("id").get_to(task.id);
    j.at("status").get_to(task.status);
    j.at("description").get_to(task.description);
}

auto operator==(const Task& lhs, const Task& rhs) -> bool
{
    return lhs.id == rhs.id && lhs.status == rhs.status && lhs.description == rhs.description;
}