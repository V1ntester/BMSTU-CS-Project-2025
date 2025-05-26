#include "TaskModel.hpp"
#include <chrono>
#include <pqxx/pqxx>
#include <stdexcept>

using namespace Components;

TaskModel::TaskModel(Storage::Manager& storageManager) : Model(storageManager) {}

void TaskModel::ValidateTask(const Task& task) const {
    if (task.title.empty()) {
        throw std::invalid_argument("Task title cannot be empty");
    }
    if (task.priority < 1 || task.priority > 3) {
        throw std::invalid_argument("Priority must be between 1 and 3");
    }
    if (task.category < 1 || task.category > 3) {
        throw std::invalid_argument("Category must be between 1 and 3");
    }
    if (task.estimatedMinutes <= 0) {
        throw std::invalid_argument("Estimated time must be positive");
    }
}

void TaskModel::VerifyTaskOwnership(int taskId, int userId, pqxx::work& transaction) {
    pqxx::result result = transaction.exec("SELECT userId FROM \"Tasks\" WHERE id = $1", pqxx::params{taskId});

    if (result.empty()) {
        throw std::runtime_error("Task not found");
    }

    if (result[0]["userId"].as<int>() != userId) {
        throw std::runtime_error("Unauthorized task access");
    }
}

bool TaskModel::IsTaskExistsForUser(int taskId, int userId) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec(
        "SELECT id FROM \"Tasks\" WHERE id = $1 AND userId = $2",
        pqxx::params{taskId, userId});

    transaction.commit();
    return !result.empty();
}

std::vector<TaskModel::Task> TaskModel::GetAllTasksForUser(int userId) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT * FROM \"Tasks\" WHERE userId = $1", pqxx::params{userId});

    std::vector<Task> tasks;
    for (const auto& row : result) {
        Task task;
        task.id = row["id"].as<int>();
        task.title = row["title"].as<std::string>();
        task.description = row["description"].as<std::string>();
        task.priority = row["priority"].as<int>();
        task.category = row["category"].as<int>();
        task.deadline = row["deadline"].as<std::string>();
        task.estimatedMinutes = row["estimatedMinutes"].as<int>();
        task.completed = row["completed"].as<bool>();
        task.userId = userId;
        tasks.push_back(task);
    }

    transaction.commit();
    return tasks;
}

bool TaskModel::CreateTaskForUser(const Task& task) {
    ValidateTask(task);
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec(
        "INSERT INTO \"Tasks\" "
        "(title, description, priority, category, deadline, estimatedMinutes, completed, userId) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7, $8) RETURNING id",
        pqxx::params{task.title, task.description, task.priority, task.category, task.deadline, task.estimatedMinutes, task.completed, task.userId});

    transaction.commit();
    return !result.empty();
}

bool TaskModel::UpdateTaskForUser(const Task& task) {
    ValidateTask(task);
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    VerifyTaskOwnership(task.id, task.userId, transaction);

    pqxx::result result = transaction.exec(
        "UPDATE \"Tasks\" SET "
        "title = $1, description = $2, priority = $3, category = $4, "
        "deadline = $5, estimatedMinutes = $6, completed = $7 "
        "WHERE id = $8 RETURNING id",
        pqxx::params{task.title, task.description, task.priority, task.category, task.deadline, task.estimatedMinutes, task.completed, task.id});

    transaction.commit();
    return !result.empty();
}

bool TaskModel::DeleteTaskForUser(int taskId, int userId) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    VerifyTaskOwnership(taskId, userId, transaction);

    pqxx::result result = transaction.exec("DELETE FROM \"Tasks\" WHERE id = $1 RETURNING id", pqxx::params{taskId});

    transaction.commit();
    return !result.empty();
}

std::vector<TaskModel::Task> TaskModel::GetTasksByPriorityForUser(int priority, int userId) {
    if (priority < 1 || priority > 3) {
        throw std::invalid_argument("Priority must be between 1 and 3");
    }

    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT * FROM \"Tasks\" WHERE priority = $1 AND userId = $2", pqxx::params{priority, userId});

    std::vector<Task> tasks;
    for (const auto& row : result) {
        Task task;
        task.id = row["id"].as<int>();
        task.title = row["title"].as<std::string>();
        task.description = row["description"].as<std::string>();
        task.priority = row["priority"].as<int>();
        task.category = row["category"].as<int>();
        task.deadline = row["deadline"].as<std::string>();
        task.estimatedMinutes = row["estimatedMinutes"].as<int>();
        task.completed = row["completed"].as<bool>();
        task.userId = userId;
        tasks.push_back(task);
    }

    transaction.commit();
    return tasks;
}

std::vector<TaskModel::Task> TaskModel::GetTasksByCategoryForUser(int category, int userId) {
    if (category < 1 || category > 3) {
        throw std::invalid_argument("Category must be between 1 and 3");
    }

    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT * FROM \"Tasks\" WHERE category = $1 AND userId = $2", pqxx::params{category, userId});

    std::vector<Task> tasks;
    for (const auto& row : result) {
        Task task;
        task.id = row["id"].as<int>();
        task.title = row["title"].as<std::string>();
        task.description = row["description"].as<std::string>();
        task.priority = row["priority"].as<int>();
        task.category = row["category"].as<int>();
        task.deadline = row["deadline"].as<std::string>();
        task.estimatedMinutes = row["estimatedMinutes"].as<int>();
        task.completed = row["completed"].as<bool>();
        task.userId = userId;
        tasks.push_back(task);
    }

    transaction.commit();
    return tasks;
}
