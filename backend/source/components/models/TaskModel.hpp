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
        size_t userId;
    };

    std::vector<Task> GetAllTasksForUser(size_t userId);
    bool CreateTaskForUser(const Task& task);
    bool UpdateTaskForUser(const Task& task);
    bool DeleteTaskForUser(int taskId, size_t userId);
    std::vector<Task> GetTasksByPriorityForUser(int priority, size_t userId);
    std::vector<Task> GetTasksByCategoryForUser(int category, size_t userId);
    bool IsTaskExistsForUser(int taskId, size_t userId);

private:
    void ValidateTask(const Task& task) const;
    void VerifyTaskOwnership(int taskId, size_t userId, pqxx::work& transaction);
};

}  // namespace Components