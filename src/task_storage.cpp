#include "task_storage.hpp"

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

} // namespace tests
