// TaskController.cpp
#include "TaskController.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <algorithm>
#include "pqxx/pqxx"

using namespace Components;
using json = nlohmann::json;

namespace {
const size_t kSuccessStatusCode = 200;
const size_t kCreatedStatusCode = 201;
const size_t kBadRequestStatusCode = 400;
}

// Вспомогательная функция сериализации Task в json
void ConvertTasksToJson(const std::vector<TaskModel::Task>& tasks, json& outJsonArray) {
    for (const auto& task : tasks) {
        json taskJson;
        taskJson["id"] = task.id;
        taskJson["title"] = task.title;
        taskJson["description"] = task.description;
        taskJson["priority"] = task.priority;
        taskJson["category"] = task.category;
        taskJson["deadline"] = task.deadline;
        taskJson["estimatedMinutes"] = task.estimatedMinutes;
        taskJson["completed"] = task.completed;
        taskJson["userId"] = task.userId;
        outJsonArray.push_back(taskJson);
    }
}

TaskController::TaskController(TaskModel& taskModel, UserModel& userModel)
    : taskModel(taskModel), userModel(userModel) {}

TaskController::~TaskController() = default;

View TaskController::GetAllTasksForUser(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("userId")) {
        return {answer};
    }

    int userId = requestBody["userId"];
    auto tasks = taskModel.GetAllTasksForUser(userId);

    json tasksJson;
    ConvertTasksToJson(tasks, tasksJson);

    answer["tasks"] = tasksJson;
    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}

View TaskController::CreateTaskForUser(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("title") || !requestBody.contains("priority") ||
        !requestBody.contains("category") || !requestBody.contains("deadline") ||
        !requestBody.contains("estimatedMinutes") || !requestBody.contains("completed") ||
        !requestBody.contains("userId")) {
        return {answer};
    }

    TaskModel::Task task;
    task.title = requestBody["title"];
    task.description = requestBody.value("description", "");
    task.priority = requestBody["priority"];
    task.category = requestBody["category"];
    task.deadline = requestBody["deadline"];
    task.estimatedMinutes = requestBody["estimatedMinutes"];
    task.completed = requestBody["completed"];
    task.userId = requestBody["userId"];

    if (taskModel.CreateTaskForUser(task)) {
        answer["code"] = kCreatedStatusCode;
        answer["message"] = "Created";
    }

    return {answer};
}

View TaskController::UpdateTaskForUser(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("id") || !requestBody.contains("title") ||
        !requestBody.contains("priority") || !requestBody.contains("category") ||
        !requestBody.contains("deadline") || !requestBody.contains("estimatedMinutes") ||
        !requestBody.contains("completed") || !requestBody.contains("userId")) {
        return {answer};
    }

    TaskModel::Task task;
    task.id = requestBody["id"];
    task.title = requestBody["title"];
    task.description = requestBody.value("description", "");
    task.priority = requestBody["priority"];
    task.category = requestBody["category"];
    task.deadline = requestBody["deadline"];
    task.estimatedMinutes = requestBody["estimatedMinutes"];
    task.completed = requestBody["completed"];
    task.userId = requestBody["userId"];

    if (taskModel.UpdateTaskForUser(task)) {
        answer["code"] = kSuccessStatusCode;
        answer["message"] = "Updated";
    }

    return {answer};
}

View TaskController::DeleteTaskForUser(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("taskId") || !requestBody.contains("userId")) {
        return {answer};
    }

    int taskId = requestBody["taskId"];
    int userId = requestBody["userId"];

    if (taskModel.DeleteTaskForUser(taskId, userId)) {
        answer["code"] = kSuccessStatusCode;
        answer["message"] = "Deleted";
    }

    return {answer};
}

View TaskController::GetTasksByPriorityForUser(const Request& request) {
    json answer;
    json requestBody;

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Bad Request";
        return {answer};
    }

    if (!requestBody.contains("priority") || !requestBody.contains("userId")) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Missing fields";
        return {answer};
    }

    int priority = requestBody["priority"];
    int userId = requestBody["userId"];
    auto tasks = taskModel.GetTasksByPriorityForUser(priority, userId);

    json tasksJson;
    ConvertTasksToJson(tasks, tasksJson);

    answer["tasks"] = tasksJson;
    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}

View TaskController::GetTasksByCategoryForUser(const Request& request) {
    json answer;
    json requestBody;

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Bad Request";
        return {answer};
    }

    if (!requestBody.contains("category") || !requestBody.contains("userId")) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Missing fields";
        return {answer};
    }

    int category = requestBody["category"];
    int userId = requestBody["userId"];
    auto tasks = taskModel.GetTasksByCategoryForUser(category, userId);

    json tasksJson;
    ConvertTasksToJson(tasks, tasksJson);

    answer["tasks"] = tasksJson;
    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}