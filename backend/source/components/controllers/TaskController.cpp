#include "TaskController.hpp"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <string>
#include "pqxx/pqxx"

using namespace Components;
using json = nlohmann::json;

namespace {
const size_t kSuccessStatusCode = 200;
const size_t kCreatedStatusCode = 201;
const size_t kBadRequestStatusCode = 400;
const size_t kForbiddenStatusCode = 403;

// bool isWrong(const TaskModel::Task& task) {
//     if (task.title.empty()) {
//         return true;
//     }

//     if (task.priority < 1 || task.priority > 3) {
//         return true;
//     }

//     if (task.category < 1 || task.category > 3) {
//         return true;
//     }

//     if (task.estimatedMinutes <= 0) {
//         return true;
//     }

//     return false;
// }

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

        outJsonArray.push_back(taskJson);
    }
}
}  // namespace

TaskController::TaskController(TaskModel& taskModel, UserModel& userModel) : taskModel(taskModel), userModel(userModel) {
}

TaskController::~TaskController() = default;

View TaskController::Get(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("login") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    if (!userModel.IdentifyByLogin(login) || !userModel.Authenticate(login, password)) {
        answer["code"] = kForbiddenStatusCode;
        answer["message"] = "Forbidden";
        return {answer};
    }

    size_t userId = userModel.GetIdByLogin(login);
    auto tasks = taskModel.Get(userId);

    json tasksJson;
    ConvertTasksToJson(tasks, tasksJson);

    answer["tasks"] = tasksJson;
    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}

View TaskController::Add(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("title") || !requestBody.contains("priority") || !requestBody.contains("category") ||
        !requestBody.contains("deadline") || !requestBody.contains("estimatedMinutes") || !requestBody.contains("completed") ||
        !requestBody.contains("login") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    if (!userModel.IdentifyByLogin(login) || !userModel.Authenticate(login, password)) {
        answer["code"] = kForbiddenStatusCode;
        answer["message"] = "Forbidden";
        return {answer};
    }

    size_t userId = userModel.GetIdByLogin(login);
    TaskModel::Task task;
    task.title = requestBody["title"];
    task.description = requestBody.value("description", "");
    task.priority = requestBody["priority"];
    task.category = requestBody["category"];
    task.deadline = requestBody["deadline"];
    task.estimatedMinutes = requestBody["estimatedMinutes"];
    task.completed = requestBody["completed"];
    task.userId = userId;

    if (taskModel.Add(task)) {
        answer["code"] = kCreatedStatusCode;
        answer["message"] = "Created";
    }

    return {answer};
}

View TaskController::Update(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("id") || !requestBody.contains("title") || !requestBody.contains("priority") || !requestBody.contains("category") ||
        !requestBody.contains("deadline") || !requestBody.contains("estimatedMinutes") || !requestBody.contains("completed") ||
        !requestBody.contains("login") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    if (!userModel.IdentifyByLogin(login) || !userModel.Authenticate(login, password)) {
        answer["code"] = kForbiddenStatusCode;
        answer["message"] = "Forbidden";
        return {answer};
    }

    size_t userId = userModel.GetIdByLogin(login);

    TaskModel::Task task;

    task.id = requestBody["id"];
    task.title = requestBody["title"];
    task.description = requestBody.value("description", "");
    task.priority = requestBody["priority"];
    task.category = requestBody["category"];
    task.deadline = requestBody["deadline"];
    task.estimatedMinutes = requestBody["estimatedMinutes"];
    task.completed = requestBody["completed"];
    task.userId = userId;

    if (taskModel.Update(task)) {
        answer["code"] = kSuccessStatusCode;
        answer["message"] = "Updated";
    }

    return {answer};
}

View TaskController::Delete(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        return {answer};
    }

    if (!requestBody.contains("taskId") || !requestBody.contains("login") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    if (!userModel.IdentifyByLogin(login) || !userModel.Authenticate(login, password)) {
        answer["code"] = kForbiddenStatusCode;
        answer["message"] = "Forbidden";
        return {answer};
    }

    size_t userId = userModel.GetIdByLogin(login);
    int taskId = requestBody["taskId"];

    if (taskModel.Delete(taskId, userId)) {
        answer["code"] = kSuccessStatusCode;
        answer["message"] = "Deleted";
    }

    return {answer};
}

View TaskController::GetByPriority(const Request& request) {
    json answer;
    json requestBody;

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Bad Request";
        return {answer};
    }

    if (!requestBody.contains("priority") || !requestBody.contains("login") || !requestBody.contains("password")) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Missing fields";
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    if (!userModel.IdentifyByLogin(login) || !userModel.Authenticate(login, password)) {
        answer["code"] = kForbiddenStatusCode;
        answer["message"] = "Forbidden";
        return {answer};
    }

    size_t userId = userModel.GetIdByLogin(login);
    int priority = requestBody["priority"];
    auto tasks = taskModel.GetByPriority(priority, userId);

    json tasksJson;
    ConvertTasksToJson(tasks, tasksJson);

    answer["tasks"] = tasksJson;
    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}

View TaskController::GetByCategory(const Request& request) {
    json answer;
    json requestBody;

    try {
        requestBody = json::parse(request.body());
    } catch (...) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Bad Request";
        return {answer};
    }

    if (!requestBody.contains("category") || !requestBody.contains("login") || !requestBody.contains("password")) {
        answer["code"] = kBadRequestStatusCode;
        answer["message"] = "Missing fields";
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    if (!userModel.IdentifyByLogin(login) || !userModel.Authenticate(login, password)) {
        answer["code"] = kForbiddenStatusCode;
        answer["message"] = "Forbidden";
        return {answer};
    }

    size_t userId = userModel.GetIdByLogin(login);
    int category = requestBody["category"];
    auto tasks = taskModel.GetByCategory(category, userId);

    json tasksJson;
    ConvertTasksToJson(tasks, tasksJson);

    answer["tasks"] = tasksJson;
    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}
