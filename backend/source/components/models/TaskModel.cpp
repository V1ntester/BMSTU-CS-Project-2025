#include "TaskModel.hpp"
#include <pqxx/pqxx>

using namespace Components;

TaskModel::TaskModel(Storage::Manager& storageManager)
    : Model(storageManager)
{
}

std::vector<TaskModel::Task> TaskModel::getAllTasks() {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT * FROM \"Tasks\"");

    std::vector<Task> tasks;
    for (auto row : result) {
        Task task;
        task.id = row["id"].as<int>();
        task.title = row["title"].as<std::string>();
        task.description = row["description"].as<std::string>();
        task.priority = row["priority"].as<int>();
        task.category = row["category"].as<int>();
        task.deadline = row["deadline"].as<std::string>();
        task.estimated_minutes = row["estimated_minutes"].as<int>();
        task.completed = row["completed"].as<bool>();
        tasks.push_back(task);
    }

    transaction.commit();
    return tasks;
}

bool TaskModel::createTask(const Task& task) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        auto result = transaction.exec(
            "INSERT INTO \"Tasks\" (title, description, priority, category, deadline, estimated_minutes, completed) "
            "VALUES (" +
            transaction.quote(task.title) + ", " +
            transaction.quote(task.description) + ", " +
            std::to_string(task.priority) + ", " +
            std::to_string(task.category) + ", " +
            transaction.quote(task.deadline) + ", " +
            std::to_string(task.estimated_minutes) + ", " +
            (task.completed ? "TRUE" : "FALSE") +
            ") RETURNING id"
        );
        transaction.commit();
        return !result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        return false;
    }
}

bool TaskModel::updateTask(const Task& task) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        auto result = transaction.exec(
            "UPDATE \"Tasks\" SET "
            "title = " + transaction.quote(task.title) + ", " +
            "description = " + transaction.quote(task.description) + ", " +
            "priority = " + std::to_string(task.priority) + ", " +
            "category = " + std::to_string(task.category) + ", " +
            "deadline = " + transaction.quote(task.deadline) + ", " +
            "estimated_minutes = " + std::to_string(task.estimated_minutes) + ", " +
            "completed = " + (task.completed ? "TRUE" : "FALSE") + " " +
            "WHERE id = " + std::to_string(task.id) + " " +
            "RETURNING id"
        );
        transaction.commit();
        return !result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        return false;
    }
}

bool TaskModel::deleteTask(int task_id) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        auto result = transaction.exec(
            "DELETE FROM \"Tasks\" WHERE id = " + std::to_string(task_id) + " " +
            "RETURNING id"
        );
        transaction.commit();
        return !result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        return false;
    }
}

std::vector<TaskModel::Task> TaskModel::getTasksByPriority(int priority) {
    auto all_tasks = getAllTasks();
    std::vector<Task> filtered_tasks;
    for (const auto& task : all_tasks) {
        if (task.priority == priority) {
            filtered_tasks.push_back(task);
        }
    }
    return filtered_tasks;
}

std::vector<TaskModel::Task> TaskModel::getTasksByCategory(int category) {
    auto all_tasks = getAllTasks();
    std::vector<Task> filtered_tasks;
    for (const auto& task : all_tasks) {
        if (task.category == category) {
            filtered_tasks.push_back(task);
        }
    }
    return filtered_tasks;
}
