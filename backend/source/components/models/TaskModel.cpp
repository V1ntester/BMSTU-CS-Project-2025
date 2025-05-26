#include "TaskModel.hpp"
#include <chrono>
#include <pqxx/pqxx>
#include <stdexcept>

using namespace Components;

TaskModel::TaskModel(Storage::Manager& storageManager) : Model(storageManager) {
}

TaskModel::~TaskModel() = default;

void TaskModel::validateTask(const Task& task) const {
    if (task.title.empty()) {
        throw std::invalid_argument("Task title cannot be empty");
    }
    if (task.priority < 1 || task.priority > 3) {
        throw std::invalid_argument("Priority must be between 1 and 3");
    }
    if (task.category < 1 || task.category > 3) {
        throw std::invalid_argument("Category must be between 1 and 3");
    }
    if (task.estimated_minutes <= 0) {
        throw std::invalid_argument("Estimated time must be positive");
    }
}

void TaskModel::verifyTaskOwnership(int task_id, int user_id, pqxx::work& transaction) {
    const std::string query =
        "SELECT user_id FROM \"Tasks\" "
        "WHERE id = " +
        std::to_string(task_id);

    pqxx::result result = transaction.exec(query);

    if (result.empty()) {
        throw std::runtime_error("Task not found");
    }

    if (result[0]["user_id"].as<int>() != user_id) {
        throw std::runtime_error("Unauthorized task access");
    }
}

std::vector<TaskModel::Task> TaskModel::getAllTasksForUser(int user_id) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        const std::string query =
            "SELECT * FROM \"Tasks\" "
            "WHERE user_id = " +
            std::to_string(user_id);

        pqxx::result result = transaction.exec(query);
        std::vector<Task> tasks;

        for (const auto& row : result) {
            Task task;
            task.id = row["id"].as<int>();
            task.title = row["title"].as<std::string>();
            task.description = row["description"].as<std::string>();
            task.priority = row["priority"].as<int>();
            task.category = row["category"].as<int>();
            task.deadline = row["deadline"].as<std::string>();
            task.estimated_minutes = row["estimated_minutes"].as<int>();
            task.completed = row["completed"].as<bool>();
            task.user_id = user_id;
            tasks.push_back(task);
        }

        transaction.commit();
        return tasks;
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Failed to get tasks: " + std::string(e.what()));
    }
}

bool TaskModel::createTaskForUser(const Task& task) {
    validateTask(task);
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        const std::string query =
            "INSERT INTO \"Tasks\" "
            "(title, description, priority, category, deadline, estimated_minutes, completed, user_id) "
            "VALUES (" +
            transaction.quote(task.title) + ", " + transaction.quote(task.description) + ", " + std::to_string(task.priority) + ", " +
            std::to_string(task.category) + ", " + transaction.quote(task.deadline) + ", " + std::to_string(task.estimated_minutes) + ", " +
            (task.completed ? "TRUE" : "FALSE") + ", " + std::to_string(task.user_id) + ") RETURNING id";

        pqxx::result result = transaction.exec(query);
        transaction.commit();
        return !result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Task creation failed: " + std::string(e.what()));
    }
}

bool TaskModel::updateTaskForUser(const Task& task) {
    validateTask(task);
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        verifyTaskOwnership(task.id, task.user_id, transaction);

        const std::string query =
            "UPDATE \"Tasks\" SET "
            "title = " +
            transaction.quote(task.title) + ", " + "description = " + transaction.quote(task.description) + ", " +
            "priority = " + std::to_string(task.priority) + ", " + "category = " + std::to_string(task.category) + ", " +
            "deadline = " + transaction.quote(task.deadline) + ", " + "estimated_minutes = " + std::to_string(task.estimated_minutes) + ", " +
            "completed = " + (task.completed ? "TRUE" : "FALSE") + " " + "WHERE id = " + std::to_string(task.id) + " " + "RETURNING id";

        pqxx::result result = transaction.exec(query);
        transaction.commit();
        return !result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Task update failed: " + std::string(e.what()));
    }
}

bool TaskModel::deleteTaskForUser(int task_id, int user_id) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        verifyTaskOwnership(task_id, user_id, transaction);

        const std::string query =
            "DELETE FROM \"Tasks\" "
            "WHERE id = " +
            std::to_string(task_id) + " " + "RETURNING id";

        pqxx::result result = transaction.exec(query);
        transaction.commit();
        return !result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Task deletion failed: " + std::string(e.what()));
    }
}

std::vector<TaskModel::Task> TaskModel::getTasksByPriorityForUser(int priority, int user_id) {
    if (priority < 1 || priority > 3) {
        throw std::invalid_argument("Priority must be between 1 and 3");
    }

    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        const std::string query =
            "SELECT * FROM \"Tasks\" "
            "WHERE priority = " +
            std::to_string(priority) + " " + "AND user_id = " + std::to_string(user_id);

        pqxx::result result = transaction.exec(query);
        std::vector<Task> tasks;

        for (const auto& row : result) {
            Task task;
            task.id = row["id"].as<int>();
            task.title = row["title"].as<std::string>();
            task.description = row["description"].as<std::string>();
            task.priority = row["priority"].as<int>();
            task.category = row["category"].as<int>();
            task.deadline = row["deadline"].as<std::string>();
            task.estimated_minutes = row["estimated_minutes"].as<int>();
            task.completed = row["completed"].as<bool>();
            task.user_id = user_id;
            tasks.push_back(task);
        }

        transaction.commit();
        return tasks;
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Failed to get tasks by priority: " + std::string(e.what()));
    }
}

std::vector<TaskModel::Task> TaskModel::getTasksByCategoryForUser(int category, int user_id) {
    if (category < 1 || category > 3) {
        throw std::invalid_argument("Category must be between 1 and 3");
    }

    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        const std::string query =
            "SELECT * FROM \"Tasks\" "
            "WHERE category = " +
            std::to_string(category) + " " + "AND user_id = " + std::to_string(user_id);

        pqxx::result result = transaction.exec(query);
        std::vector<Task> tasks;

        for (const auto& row : result) {
            Task task;
            task.id = row["id"].as<int>();
            task.title = row["title"].as<std::string>();
            task.description = row["description"].as<std::string>();
            task.priority = row["priority"].as<int>();
            task.category = row["category"].as<int>();
            task.deadline = row["deadline"].as<std::string>();
            task.estimated_minutes = row["estimated_minutes"].as<int>();
            task.completed = row["completed"].as<bool>();
            task.user_id = user_id;
            tasks.push_back(task);
        }

        transaction.commit();
        return tasks;
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Failed to get tasks by category: " + std::string(e.what()));
    }
}
