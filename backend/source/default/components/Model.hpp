#pragma once

#include <memory>
#include "../../storage/Manager.hpp"

namespace Components {
class Model {
 public:
    Model(std::shared_ptr<Storage::Manager> manager);

    ~Model();

 private:
    std::shared_ptr<Storage::Manager> manager;
};
}  // namespace Components
