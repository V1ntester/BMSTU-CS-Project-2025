#pragma once

#include "../../default/components/Controller.hpp"
#include "../models/TaskModel.hpp"
#include <nlohmann/json.hpp>

namespace Components {

class TaskController : public Contoller {
public:
    explicit TaskController(TaskModel& taskModel);
    ~TaskController() override = default;

    View CreateTask(const nlohmann::json& taskData);
    View UpdateTask(const nlohmann::json& taskData);
    View DeleteTask(int taskId, int userId);
    View GetAllTasks(int userId);
    View GetTasksByPriority(int priority, int userId);
    View GetTasksByCategory(int category, int userId);

private:
    TaskModel& taskModel;

    void validateTaskData(const nlohmann::json& taskData) const;
    TaskModel::Task jsonToTask(const nlohmann::json& taskData) const;
    nlohmann::json taskToJson(const TaskModel::Task& task) const;
};

} // namespace Components