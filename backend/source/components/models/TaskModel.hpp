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
    };

    std::vector<Task> getAllTasks();
    bool createTask(const Task& task);
    bool updateTask(const Task& task);
    bool deleteTask(int task_id);
    std::vector<Task> getTasksByPriority(int priority);
    std::vector<Task> getTasksByCategory(int category);
};
} // namespace Components
