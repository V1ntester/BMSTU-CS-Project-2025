#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../../default/components/Model.hpp"

namespace Components {
class TestModel : public Model {
 public:
    TestModel(Storage::Manager& storageManager);
    
    ~TestModel() override;

    std::vector<std::string> GetAllLoginsOfUsersInDataBase();
};    
}
