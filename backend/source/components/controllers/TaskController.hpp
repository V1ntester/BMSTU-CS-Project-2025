#pragma once

#include "../../default/components/Controller.hpp"
#include "../models/TaskModel.hpp"
#include "../models/UserModel.hpp"

namespace Components {

class TaskController {
public:
    TaskController(TaskModel& taskModel, UserModel& userModel);
    ~TaskController();

    View GetAllTasksForUser(const Request& request);
    View CreateTaskForUser(const Request& request);
    View UpdateTaskForUser(const Request& request);
    View DeleteTaskForUser(const Request& request);
    View GetTasksByPriorityForUser(const Request& request);
    View GetTasksByCategoryForUser(const Request& request);

private:
    TaskModel& taskModel;
    UserModel& userModel;

};

} // namespace Components
