#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../../default/components/Model.hpp"

namespace Components {

class TaskModel : public Model {
 public:
    explicit TaskModel(Storage::Manager& storageManager);
    ~TaskModel() = default;

    struct Task {
        int id;
        std::string title;
        std::string description;
        int priority;
        int category;
        std::string deadline;
        int estimated_minutes;
        bool completed;
        int user_id;
    };

    std::vector<Task> getAllTasksForUser(int user_id);
    bool createTaskForUser(const Task& task);
    bool updateTaskForUser(const Task& task);
    bool deleteTaskForUser(int task_id, int user_id);
    std::vector<Task> getTasksByPriorityForUser(int priority, int user_id);
    std::vector<Task> getTasksByCategoryForUser(int category, int user_id);

 private:
    void validateTask(const Task& task) const;
    void verifyTaskOwnership(int task_id, int user_id, pqxx::work& transaction);
};

}  // namespace Components
