#include "task_storage.hpp"

#include <print>
#include <fstream>
#include <ranges>

#include <nlohmann/json.hpp>

namespace {

using json = nlohmann::json;

auto tasks_from_json(const json& tasks_json) -> std::unordered_map<TaskID, Task>
{
    if (!tasks_json.is_array()) {
        throw std::runtime_error("invalid tasks json format, expected array.");
    }

    std::unordered_map<TaskID, Task> tasks;

    for (const auto& task_json : tasks_json) {
        auto task = task_json.template get<Task>();
        tasks.emplace(task.id, task);
    }

    return tasks;
}

auto tasks_to_json(const std::unordered_map<TaskID, Task>& tasks) -> json
{
    json tasks_json = json::array();

    for (const auto& task: tasks | std::views::values) {
        json task_json = task;
        tasks_json.push_back(task_json);
    }

    return tasks_json;
}

auto get_max_id(const std::unordered_map<TaskID, Task>& tasks) -> TaskID
{
    if (tasks.empty()) {
        return 0;
    }

    return std::ranges::max(tasks | std::views::keys);
}

} // anonymous namespace

TaskStorage::TaskStorage(std::optional<std::filesystem::path> file_storage)
    : file_storage{ file_storage }
{
    if (!file_storage) {
        return;
    }

    auto file = std::fstream{*file_storage, std::ios::in};

    if (!file.is_open()) {
        std::println("failed to open file storage. starting with empty tasks.");
        return;
    }

    import_tasks_from_stream(file);
}

auto TaskStorage::export_tasks_to_stream(std::ostream& os) const -> void
{
    json tasks_json = tasks_to_json(tasks);

    os << tasks_json.dump(4);
}

auto TaskStorage::import_tasks_from_stream(std::istream& is) -> void
{
    json tasks_json;
    is >> tasks_json;

    tasks = tasks_from_json(tasks_json);

    next_id = get_max_id(tasks) + 1;
}

auto TaskStorage::save_tasks() -> void
{
    if (!file_storage) {
        return;
    }

    auto file = std::fstream{*file_storage, std::ios::out | std::ios::trunc};

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file storage.");
    }

    export_tasks_to_stream(file);
}

auto TaskStorage::add_task(std::string_view description) -> TaskID
{
    auto id = next_id++;

    tasks.emplace(id, Task{
        id: id, 
        status: TaskStatus::Todo,
        description: std::string{description}, 
    });

    return id;
}

auto TaskStorage::get_task(TaskID id) const -> std::optional<std::reference_wrapper<const Task>>
{
    auto it = tasks.find(id);

    if (it == tasks.end()) {
        return std::nullopt;
    }

    return it->second;
}

auto TaskStorage::update_task(TaskID id, const TaskUpdate& update) -> std::optional<std::reference_wrapper<const Task>>
{
    auto it = tasks.find(id);

    if (it == tasks.end()) {
        return std::nullopt;
    }

    if (update.description) {
        it->second.description = *update.description;
    }

    if (update.status) {
        it->second.status = *update.status;
    }

    return it->second;
}

auto TaskStorage::delete_task(TaskID id) -> void
{
    tasks.erase(id);
}

auto TaskStorage::get_tasks_map() const -> const std::unordered_map<TaskID, Task>&
{
    return tasks;
}

#ifdef UNIT_TESTS

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <algorithm>
namespace tests {

TEST_CASE("TaskStorage can add multiple tasks with unique ids", "[task_storage]")
{
    TaskStorage task_storage;

    REQUIRE(task_storage.get_tasks_map().size() == 0);

    auto task_id_1 = task_storage.add_task("Test task 1");
    auto task_id_2 = task_storage.add_task("Test task 2");

    REQUIRE(task_storage.get_tasks_map().size() == 2);

    REQUIRE(task_id_1 != task_id_2);
}

TEST_CASE("TaskStorage can add and get tasks using their ids", "[task_storage]")
{
    TaskStorage task_storage;

    REQUIRE(task_storage.get_tasks_map().size() == 0);
    
    auto task_id_1 = task_storage.add_task("Test task 1");

    REQUIRE(task_storage.get_tasks_map().size() == 1);

    auto task_id_2 = task_storage.add_task("Test task 2");

    REQUIRE(task_storage.get_tasks_map().size() == 2);

    auto& task_1 = task_storage.get_task(task_id_1)->get();

    REQUIRE(task_1.description == "Test task 1");
    REQUIRE(task_1.status == TaskStatus::Todo);

    auto& task_2 = task_storage.get_task(task_id_2)->get();

    REQUIRE(task_2.description == "Test task 2");
    REQUIRE(task_2.status == TaskStatus::Todo);
}

TEST_CASE("TaskStorage can delete a task", "[task_storage]")
{
    TaskStorage task_storage;

    REQUIRE(task_storage.get_tasks_map().size() == 0);

    auto task_id = task_storage.add_task("Test task");

    REQUIRE(task_storage.get_tasks_map().size() == 1);

    task_storage.delete_task(task_id);

    REQUIRE(task_storage.get_tasks_map().size() == 0);

    auto result = task_storage.get_task(task_id);

    REQUIRE(!result);
}

TEST_CASE("TaskStorage can update all of a tasks's field at once", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id = task_storage.add_task("Test task");

    auto result = task_storage.update_task(task_id, TaskUpdate{ description: "Updated task", status: TaskStatus::Done });

    REQUIRE(result);
    REQUIRE(result->get().description == "Updated task");
    REQUIRE(result->get().status == TaskStatus::Done);
}

TEST_CASE("TaskStorage can update a task's field independently", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id = task_storage.add_task("Test task");

    auto result = task_storage.update_task(task_id, TaskUpdate{ description: "Updated task" });

    REQUIRE(result);
    REQUIRE(result->get().description == "Updated task");
    REQUIRE(result->get().status == TaskStatus::Todo);

    result = task_storage.update_task(task_id, TaskUpdate{ status: TaskStatus::Done });

    REQUIRE(result);
    REQUIRE(result->get().description == "Updated task");
    REQUIRE(result->get().status == TaskStatus::Done);
}

// VIEWS

TEST_CASE("TaskStorage can provide a view with all tasks", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id_1 = task_storage.add_task("Test task 1");
    auto task_id_2 = task_storage.add_task("Test task 2");

    task_storage.update_task(task_id_1, TaskUpdate{ status: TaskStatus::Done });

    auto all_tasks = task_storage.view_all_tasks() | std::ranges::to<std::vector<Task>>();

    auto all_tasks_task_1 = std::find_if(all_tasks.begin(), all_tasks.end(), [task_id_1](const Task& task) {
        return task.id == task_id_1;
    });

    auto all_tasks_task_2 = std::find_if(all_tasks.begin(), all_tasks.end(), [task_id_2](const Task& task) {
        return task.id == task_id_2;
    });

    REQUIRE(all_tasks.size() == 2);
    REQUIRE(all_tasks.size() == task_storage.get_tasks_map().size());
    REQUIRE(all_tasks_task_1 != all_tasks.end());
    REQUIRE(all_tasks_task_1->description == "Test task 1");
    REQUIRE(all_tasks_task_2 != all_tasks.end());
    REQUIRE(all_tasks_task_2->description == "Test task 2");
}

TEST_CASE("TaskStorage can provide a view with all tasks in the Todo status", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id_1 = task_storage.add_task("Test task 1");
    auto task_id_2 = task_storage.add_task("Test task 2");

    task_storage.update_task(task_id_1, TaskUpdate{ status: TaskStatus::Done });

    auto all_tasks_todo = task_storage.view_all_tasks_todo() | std::ranges::to<std::vector<Task>>();   

    auto all_tasks_todo_task_1 = std::find_if(all_tasks_todo.begin(), all_tasks_todo.end(), [task_id_1](const Task& task) {
        return task.id == task_id_1;
    });

    auto all_tasks_todo_task_2 = std::find_if(all_tasks_todo.begin(), all_tasks_todo.end(), [task_id_2](const Task& task) {
        return task.id == task_id_2;
    });

    REQUIRE(all_tasks_todo.size() == 1);
    REQUIRE(all_tasks_todo_task_1 == all_tasks_todo.end());
    REQUIRE(all_tasks_todo_task_2 != all_tasks_todo.end());
    REQUIRE(all_tasks_todo_task_2->description == "Test task 2");
}

TEST_CASE("TaskStorage can provide a view with all tasks in the InProgress status", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id_1 = task_storage.add_task("Test task 1");
    auto task_id_2 = task_storage.add_task("Test task 2");

    task_storage.update_task(task_id_1, TaskUpdate{ status: TaskStatus::Done });
    task_storage.update_task(task_id_2, TaskUpdate{ status: TaskStatus::InProgress });

    auto all_tasks_in_progress = task_storage.view_all_tasks_in_progress() | std::ranges::to<std::vector<Task>>();

    auto all_tasks_in_progress_task_1 = std::find_if(all_tasks_in_progress.begin(), all_tasks_in_progress.end(), [task_id_1](const Task& task) {
        return task.id == task_id_1;
    });

    auto all_tasks_in_progress_task_2 = std::find_if(all_tasks_in_progress.begin(), all_tasks_in_progress.end(), [task_id_2](const Task& task) {
        return task.id == task_id_2;
    });

    REQUIRE(all_tasks_in_progress.size() == 1);
    REQUIRE(all_tasks_in_progress_task_1 == all_tasks_in_progress.end());
    REQUIRE(all_tasks_in_progress_task_2 != all_tasks_in_progress.end());
    REQUIRE(all_tasks_in_progress_task_2->description == "Test task 2");
}

TEST_CASE("TaskStorage can provide a view with all tasks in the Done status", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id_1 = task_storage.add_task("Test task 1");
    auto task_id_2 = task_storage.add_task("Test task 2");

    task_storage.update_task(task_id_1, TaskUpdate{ status: TaskStatus::Done });
    task_storage.update_task(task_id_2, TaskUpdate{ status: TaskStatus::InProgress });

    auto all_tasks_done = task_storage.view_all_tasks_done() | std::ranges::to<std::vector<Task>>();

    auto all_tasks_done_task_1 = std::find_if(all_tasks_done.begin(), all_tasks_done.end(), [task_id_1](const Task& task) {
        return task.id == task_id_1;
    });

    auto all_tasks_done_task_2 = std::find_if(all_tasks_done.begin(), all_tasks_done.end(), [task_id_2](const Task& task) {
        return task.id == task_id_2;
    });

    REQUIRE(all_tasks_done.size() == 1);
    REQUIRE(all_tasks_done_task_1 != all_tasks_done.end());
    REQUIRE(all_tasks_done_task_1->description == "Test task 1");
    REQUIRE(all_tasks_done_task_2 == all_tasks_done.end());
}

TEST_CASE("TaskStorage can export tasks into valid json", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id_1 = task_storage.add_task("Test task 1");
    auto task_id_2 = task_storage.add_task("Test task 2");

    task_storage.update_task(task_id_1, TaskUpdate{ status: TaskStatus::Done });

    std::stringstream ss;

    task_storage.export_tasks_to_stream(ss);

    json tasks_json;
    ss >> tasks_json;

    REQUIRE(tasks_json.is_array());
    REQUIRE(tasks_json.size() == 2);

    auto task_1_json = *std::find_if(tasks_json.begin(), tasks_json.end(), [task_id_1](const json& task_json) {
        return task_json["id"] == task_id_1;
    });
    auto task_2_json = *std::find_if(tasks_json.begin(), tasks_json.end(), [task_id_2](const json& task_json) {
        return task_json["id"] == task_id_2;
    });

    REQUIRE(task_1_json["id"] == task_id_1);
    REQUIRE(task_1_json["status"] == "Done");
    REQUIRE(task_1_json["description"] == "Test task 1");

    REQUIRE(task_2_json["id"] == task_id_2);
    REQUIRE(task_2_json["status"] == "Todo");
    REQUIRE(task_2_json["description"] == "Test task 2");
}

TEST_CASE("TaskStorage can export and import tasks", "[task_storage]")
{
    TaskStorage task_storage;

    auto task_id_1 = task_storage.add_task("Test task 1");
    task_storage.add_task("Test task 2");

    task_storage.update_task(task_id_1, TaskUpdate{ status: TaskStatus::Done });

    std::stringstream ss;

    task_storage.export_tasks_to_stream(ss);

    TaskStorage task_storage_imported;

    task_storage_imported.import_tasks_from_stream(ss);

    auto& all_tasks = task_storage.get_tasks_map();
    auto& all_tasks_imported = task_storage_imported.get_tasks_map();

    REQUIRE(all_tasks.size() == all_tasks_imported.size());

    for (const auto& [id, task] : all_tasks) {
        auto it = all_tasks_imported.find(id);

        REQUIRE(it != all_tasks_imported.end());
        REQUIRE(it->second == task);
    }
}

} // namespace tests

#endif // UNIT_TESTS