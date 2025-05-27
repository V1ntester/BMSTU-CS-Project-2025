#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../../default/components/Model.hpp"

namespace Components {

class TaskModel : public Model {
public:
    explicit TaskModel(Storage::Manager& storageManager);
    ~TaskModel() override;

    struct Task {
        int id;
        std::string title;
        std::string description;
        int priority;
        int category;
        std::string deadline;
        int estimatedMinutes;
        bool completed;
        int userId;
    };

    std::vector<Task> GetAllTasksForUser(int userId);
    bool CreateTaskForUser(const Task& task);
    bool UpdateTaskForUser(const Task& task);
    bool DeleteTaskForUser(int taskId, int userId);
    std::vector<Task> GetTasksByPriorityForUser(int priority, int userId);
    std::vector<Task> GetTasksByCategoryForUser(int category, int userId);
    bool IsTaskExistsForUser(int taskId, int userId);

private:
    void ValidateTask(const Task& task) const;
    void VerifyTaskOwnership(int taskId, int userId, pqxx::work& transaction);
};

}  // namespace Components