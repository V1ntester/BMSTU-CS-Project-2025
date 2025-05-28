#pragma once

#include "../../default/components/Controller.hpp"
#include "../models/TaskModel.hpp"
#include "../models/UserModel.hpp"

namespace Components {

class TaskController {
 public:
    TaskController(TaskModel& taskModel, UserModel& userModel);

    ~TaskController();

    View Get(const Request& request);
    View GetByPriority(const Request& request);
    View GetByCategory(const Request& request);

    View Add(const Request& request);

    View Delete(const Request& request);

    View Update(const Request& request);

 private:
    TaskModel& taskModel;
    UserModel& userModel;
};

}  // namespace Components
