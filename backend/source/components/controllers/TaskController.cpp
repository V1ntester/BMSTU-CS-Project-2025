#include "TaskController.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include "pqxx/pqxx"

using namespace Components;
using json = nlohmann::json;

namespace {
// const size_t kSuccessStatusCode = 200;
// const size_t kBadRequestStatusCode = 400;
// // const size_t kUnauthorizedStatusCode = 401;
// const size_t kNotFoundStatusCode = 404;
}

TaskController::TaskController(TaskModel& taskModel, UserModel& userModel) 
    : taskModel(taskModel), userModel(userModel) {
}

TaskController::~TaskController() = default;

// int TaskController::ExtractUserIdFromRequest(const Request& request) const {
//     // try {
//     //     auto authHeader = request.find("Authorization");
//     //     // както надо репализовать получение реквеста из вне (я не понимаю как он снаружи к нам - ыть)
//     // }

//     return 0;
// }

// TaskModel::Task TaskController::ParseTaskFromRequest(const Request& request, bool requireId) const {
//     try {
//         json body = json::parse(request.body());
//         TaskModel::Task task;

//         if (requireId) {
//             task.id = body["id"].get<int>();
//         }
        
//         task.title = body["title"].get<std::string>();
//         task.description = body["description"].get<std::string>();
//         task.priority = body["priority"].get<int>();
//         task.category = body["category"].get<int>();
//         task.deadline = body["deadline"].get<std::string>();
//         task.estimatedMinutes = body["estimatedMinutes"].get<int>();
//         task.completed = body.value("completed", false);

//         return task;
//     } catch (const std::exception& e) {
//         throw std::runtime_error("Invalid task data: " + std::string(e.what()));
//     }
// }

// View TaskController::CreateTask(const Request& request) {
//     json response;
    
//     try {
//         int userId = ExtractUserIdFromRequest(request);
//         TaskModel::Task task = ParseTaskFromRequest(request);
//         task.userId = userId;

//         if (taskModel.CreateTaskForUser(task)) {
//             response["code"] = kSuccessStatusCode;
//             response["message"] = "Task created successfully";
//         } else {
//             response["code"] = kBadRequestStatusCode;
//             response["message"] = "Failed to create task";
//         }
//     } catch (const std::exception& e) {
//         response["code"] = kBadRequestStatusCode;
//         response["message"] = e.what();
//     }

//     return response;
// }

// View TaskController::UpdateTask(const Request& request) {
//     json response;
    
//     try {
//         int userId = ExtractUserIdFromRequest(request);
//         TaskModel::Task task = ParseTaskFromRequest(request, true);
//         task.userId = userId;

//         if (!taskModel.IsTaskExistsForUser(task.id, userId)) {
//             response["code"] = kNotFoundStatusCode;
//             response["message"] = "Task not found";
//             return response;
//         }

//         if (taskModel.UpdateTaskForUser(task)) {
//             response["code"] = kSuccessStatusCode;
//             response["message"] = "Task updated successfully";
//         } else {
//             response["code"] = kBadRequestStatusCode;
//             response["message"] = "Failed to update task";
//         }
//     } catch (const std::exception& e) {
//         response["code"] = kBadRequestStatusCode;
//         response["message"] = e.what();
//     }

//     return response;
// }

// View TaskController::DeleteTask(const Request& request) {
//     json response;
    
//     try {
//         int userId = ExtractUserIdFromRequest(request);
//         json body = json::parse(request.body());
//         int taskId = body["id"].get<int>();

//         if (!taskModel.IsTaskExistsForUser(taskId, userId)) {
//             response["code"] = kNotFoundStatusCode;
//             response["message"] = "Task not found";
//             return response;
//         }

//         if (taskModel.DeleteTaskForUser(taskId, userId)) {
//             response["code"] = kSuccessStatusCode;
//             response["message"] = "Task deleted successfully";
//         } else {
//             response["code"] = kBadRequestStatusCode;
//             response["message"] = "Failed to delete task";
//         }
//     } catch (const std::exception& e) {
//         response["code"] = kBadRequestStatusCode;
//         response["message"] = e.what();
//     }

//     return response;
// }

// View TaskController::GetAllTasks(const Request& request) {
//     json response;
    
//     try {
//         int userId = ExtractUserIdFromRequest(request);
//         auto tasks = taskModel.GetAllTasksForUser(userId);

//         response["code"] = kSuccessStatusCode;
//         response["message"] = "Success";
//         response["tasks"] = json::array();

//         for (const auto& task : tasks) {
//             json taskJson;
//             taskJson["id"] = task.id;
//             taskJson["title"] = task.title;
//             taskJson["description"] = task.description;
//             taskJson["priority"] = task.priority;
//             taskJson["category"] = task.category;
//             taskJson["deadline"] = task.deadline;
//             taskJson["estimatedMinutes"] = task.estimatedMinutes;
//             taskJson["completed"] = task.completed;
//             response["tasks"].push_back(taskJson);
//         }
//     } catch (const std::exception& e) {
//         response["code"] = kBadRequestStatusCode;
//         response["message"] = e.what();
//     }

//     return response;
// }

// View TaskController::GetTasksByPriority(const Request& request) {
//     json response;
    
//     try {
//         int userId = ExtractUserIdFromRequest(request);
//         json body = json::parse(request.body());
//         int priority = body["priority"].get<int>();

//         auto tasks = taskModel.GetTasksByPriorityForUser(priority, userId);

//         response["code"] = kSuccessStatusCode;
//         response["message"] = "Success";
//         response["tasks"] = json::array();

//         for (const auto& task : tasks) {
//             json taskJson;
//             taskJson["id"] = task.id;
//             taskJson["title"] = task.title;
//             taskJson["description"] = task.description;
//             taskJson["priority"] = task.priority;
//             taskJson["category"] = task.category;
//             taskJson["deadline"] = task.deadline;
//             taskJson["estimatedMinutes"] = task.estimatedMinutes;
//             taskJson["completed"] = task.completed;
//             response["tasks"].push_back(taskJson);
//         }
//     } catch (const std::exception& e) {
//         response["code"] = kBadRequestStatusCode;
//         response["message"] = e.what();
//     }

//     return response;
// }

// View TaskController::GetTasksByCategory(const Request& request) {
//     json response;
    
//     try {
//         int userId = ExtractUserIdFromRequest(request);
//         json body = json::parse(request.body());
//         int category = body["category"].get<int>();

//         auto tasks = taskModel.GetTasksByCategoryForUser(category, userId);

//         response["code"] = kSuccessStatusCode;
//         response["message"] = "Success";
//         response["tasks"] = json::array();

//         for (const auto& task : tasks) {
//             json taskJson;
//             taskJson["id"] = task.id;
//             taskJson["title"] = task.title;
//             taskJson["description"] = task.description;
//             taskJson["priority"] = task.priority;
//             taskJson["category"] = task.category;
//             taskJson["deadline"] = task.deadline;
//             taskJson["estimatedMinutes"] = task.estimatedMinutes;
//             taskJson["completed"] = task.completed;
//             response["tasks"].push_back(taskJson);
//         }
//     } catch (const std::exception& e) {
//         response["code"] = kBadRequestStatusCode;
//         response["message"] = e.what();
//     }

//     return response;
// }
