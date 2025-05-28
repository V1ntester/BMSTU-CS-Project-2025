#include "TaskModel.hpp"
#include <chrono>
#include <pqxx/pqxx>
#include <stdexcept>

using namespace Components;

TaskModel::TaskModel(Storage::Manager& storageManager) : Model(storageManager) {
}

TaskModel::~TaskModel() = default;

bool TaskModel::IsExists(int taskId, size_t userId) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT id FROM \"Tasks\" WHERE id = $1 AND userId = $2", pqxx::params{taskId, userId});

    transaction.commit();
    return !result.empty();
}

std::vector<TaskModel::Task> TaskModel::Get(size_t userId) {
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

bool TaskModel::Add(const Task& task) {
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

bool TaskModel::Update(const Task& task) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec(
        "UPDATE \"Tasks\" SET "
        "title = $1, description = $2, priority = $3, category = $4, "
        "deadline = $5, estimatedMinutes = $6, completed = $7 "
        "WHERE id = $8 AND userid = $9 RETURNING id",
        pqxx::params{task.title, task.description, task.priority, task.category, task.deadline, task.estimatedMinutes, task.completed, task.id,
                     task.userId});

    transaction.commit();
    return !result.empty();
}

bool TaskModel::Delete(int taskId, size_t userId) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("DELETE FROM \"Tasks\" WHERE id = $1 AND userid = $2 RETURNING id", pqxx::params{taskId, userId});

    transaction.commit();
    return !result.empty();
}

std::vector<TaskModel::Task> TaskModel::GetByPriority(int priority, size_t userId) {
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

std::vector<TaskModel::Task> TaskModel::GetByCategory(int category, size_t userId) {
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
