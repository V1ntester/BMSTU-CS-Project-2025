#pragma once

#include "../../default/components/Controller.hpp"
#include "../models/TaskModel.hpp"
#include "../models/UserModel.hpp"

namespace Components {

class TaskController : public Controller {
public:
    TaskController(TaskModel& taskModel, UserModel& userModel);
    
    ~TaskController() override;

    // View CreateTask(const Request& request);
    // View UpdateTask(const Request& request);
    // View DeleteTask(const Request& request);
    // View GetAllTasks(const Request& request);
    // View GetTasksByPriority(const Request& request);
    // View GetTasksByCategory(const Request& request);

private:
    TaskModel& taskModel;
    UserModel& userModel;

    // int ExtractUserIdFromRequest(const Request& request) const;
    // TaskModel::Task ParseTaskFromRequest(const Request& request, bool requireId = false) const;
};

}  // namespace Components
