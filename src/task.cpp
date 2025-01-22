#include "task.hpp"

#include <string_view>
#include <sstream>

#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

namespace {

auto parse_time(const std::string& fmt, const std::string& time_str) -> std::chrono::system_clock::time_point
{
    auto is = std::istringstream{time_str};
    std::chrono::system_clock::time_point tp;
    is >> std::chrono::parse(fmt, tp);
    return tp;
}

} // anonymous namespace

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
    auto created_at = std::format("{:" TIME_FORMAT "}", task.created_at);
    auto updated_at = std::format("{:" TIME_FORMAT "}", task.updated_at);

    j = json{
        {"id", task.id},
        {"status", task.status},
        {"description", task.description},
        {"created_at", created_at},
        {"updated_at", updated_at},
    };
}

auto from_json(const json& j, Task& task) -> void
{
    j.at("id").get_to(task.id);
    j.at("status").get_to(task.status);
    j.at("description").get_to(task.description);

    auto created_at = j.at("created_at").get<std::string>();
    task.created_at = parse_time(TIME_FORMAT, created_at);

    auto updated_at = j.at("updated_at").get<std::string>();
    task.updated_at = parse_time(TIME_FORMAT, updated_at);
}

auto operator==(const Task& lhs, const Task& rhs) -> bool
{
    return lhs.id == rhs.id 
        && lhs.status == rhs.status 
        && lhs.description == rhs.description
        && lhs.created_at == rhs.created_at
        && lhs.updated_at == rhs.updated_at;
}

auto operator<<(std::ostream &os, const TaskStatus &status) -> std::ostream &
{
    switch (status) {
    case TaskStatus::Todo:
        return os << "Todo";
    case TaskStatus::Done:
        return os << "Done";
    case TaskStatus::InProgress:
        return os << "InProgress";
    default:
        return os << "Unknown";
    }
}

auto operator<<(std::ostream &os, const Task &task) -> std::ostream &
{
    return os << "Task(id:" << task.id 
        << ", status:" << task.status 
        << ", description:" << task.description 
        << ", created_at:" << task.created_at
        << ", updated_at:" << task.updated_at
        << ")";
}
