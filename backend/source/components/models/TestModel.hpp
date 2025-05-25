#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../default/components/Model.hpp"

namespace Components {
class TestModel : public Model {
 public:
    explicit TestModel(Storage::Manager& storageManager);

    ~TestModel() override;

    std::vector<std::string> GetAllLoginsOfUsersInDataBase();
};
}  // namespace Components
