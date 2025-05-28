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

    std::vector<Task> Get(size_t userId);
    std::vector<Task> GetByPriority(int priority, size_t userId);
    std::vector<Task> GetByCategory(int category, size_t userId);

    bool Add(const Task& task);
    bool Delete(int taskId, size_t userId);

    bool Update(const Task& task);

 private:
    bool IsExists(int taskId, size_t userId);
};

}  // namespace Components
