#include "TaskController.hpp"
#include <stdexcept>

using namespace Components;
using json = nlohmann::json;

namespace {
    const size_t kSuccessStatusCode = 200;
    const size_t kBadRequestStatusCode = 400;
    const size_t kNotFoundStatusCode = 404;
    const size_t kInternalErrorStatusCode = 500;
}

TaskController::TaskController(TaskModel& taskModel) 
    : taskModel(taskModel) {}

void TaskController::validateTaskData(const json& taskData) const {
    if (!taskData.contains("title") || taskData["title"].empty()) {
        throw std::invalid_argument("Title is required");
    }
    if (!taskData.contains("priority") || 
        (taskData["priority"] < 1 || taskData["priority"] > 3)) {
        throw std::invalid_argument("Priority must be 1 (low), 2 (medium) or 3 (high)");
    }
    if (!taskData.contains("category") || 
        (taskData["category"] < 1 || taskData["category"] > 3)) {
        throw std::invalid_argument("Category must be 1 (personal), 2 (work) or 3 (study)");
    }
    if (!taskData.contains("estimated_minutes") || 
        (taskData["estimated_minutes"] <= 0)) {
        throw std::invalid_argument("Estimated time must be positive");
    }
    if (!taskData.contains("user_id")) {
        throw std::invalid_argument("User ID is required");
    }
}

TaskModel::Task TaskController::jsonToTask(const json& taskData) const {
    TaskModel::Task task;
    if (taskData.contains("id")) { 
        task.id = taskData["id"]; 
    }
    task.title = taskData["title"];
    task.description = taskData.value("description", "");
    task.priority = taskData["priority"];
    task.category = taskData["category"];
    task.deadline = taskData.value("deadline", "");
    task.estimated_minutes = taskData["estimated_minutes"];
    task.completed = taskData.value("completed", false);
    task.user_id = taskData["user_id"];
    return task;
}

json TaskController::taskToJson(const TaskModel::Task& task) const {
    return {
        {"id", task.id},
        {"title", task.title},
        {"description", task.description},
        {"priority", task.priority},
        {"category", task.category},
        {"deadline", task.deadline},
        {"estimated_minutes", task.estimated_minutes},
        {"completed", task.completed},
        {"user_id", task.user_id}
    };
}

View TaskController::CreateTask(const json& taskData) {
    json response;
    try {
        validateTaskData(taskData);
        TaskModel::Task task = jsonToTask(taskData);
        bool success = taskModel.createTaskForUser(task);
        
        if (success) {
            response["code"] = kSuccessStatusCode;
            response["message"] = "Task created successfully";
        } else {
            response["code"] = kInternalErrorStatusCode;
            response["message"] = "Failed to create task";
        }
    } catch (const std::exception& e) {
        response["code"] = kBadRequestStatusCode;
        response["message"] = e.what();
    }
    return {response};
}

View TaskController::UpdateTask(const json& taskData) {
    json response;
    try {
        validateTaskData(taskData);
        TaskModel::Task task = jsonToTask(taskData);
        bool success = taskModel.updateTaskForUser(task);
        
        if (success) {
            response["code"] = kSuccessStatusCode;
            response["message"] = "Task updated successfully";
        } else {
            response["code"] = kNotFoundStatusCode;
            response["message"] = "Task not found or update failed";
        }
    } catch (const std::exception& e) {
        response["code"] = kBadRequestStatusCode;
        response["message"] = e.what();
    }
    return {response};
}

View TaskController::DeleteTask(int taskId, int userId) {
    json response;
    try {
        bool success = taskModel.deleteTaskForUser(taskId, userId);
        
        if (success) {
            response["code"] = kSuccessStatusCode;
            response["message"] = "Task deleted successfully";
        } else {
            response["code"] = kNotFoundStatusCode;
            response["message"] = "Task not found or delete failed";
        }
    } catch (const std::exception& e) {
        response["code"] = kBadRequestStatusCode;
        response["message"] = e.what();
    }
    return {response};
}

View TaskController::GetAllTasks(int userId) {
    json response;
    try {
        auto tasks = taskModel.getAllTasksForUser(userId);
        json tasksJson = json::array();
        
        for (const auto& task : tasks) {
            tasksJson.push_back(taskToJson(task));
        }
        
        response["code"] = kSuccessStatusCode;
        response["tasks"] = tasksJson;
    } catch (const std::exception& e) {
        response["code"] = kInternalErrorStatusCode;
        response["message"] = e.what();
    }
    return {response};
}

View TaskController::GetTasksByPriority(int priority, int userId) {
    json response;
    try {
        auto tasks = taskModel.getTasksByPriorityForUser(priority, userId);
        json tasksJson = json::array();
        
        for (const auto& task : tasks) {
            tasksJson.push_back(taskToJson(task));
        }
        
        response["code"] = kSuccessStatusCode;
        response["tasks"] = tasksJson;
    } catch (const std::exception& e) {
        response["code"] = kBadRequestStatusCode;
        response["message"] = e.what();
    }
    return {response};
}

View TaskController::GetTasksByCategory(int category, int userId) {
    json response;
    try {
        auto tasks = taskModel.getTasksByCategoryForUser(category, userId);
        json tasksJson = json::array();
        
        for (const auto& task : tasks) {
            tasksJson.push_back(taskToJson(task));
        }
        
        response["code"] = kSuccessStatusCode;
        response["tasks"] = tasksJson;
    } catch (const std::exception& e) {
        response["code"] = kBadRequestStatusCode;
        response["message"] = e.what();
    }
    return {response};
}