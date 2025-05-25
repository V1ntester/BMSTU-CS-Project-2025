#pragma once

#include <memory>
#include "../../storage/Manager.hpp"

namespace Components {
class Model {
 public:
    Model(Storage::Manager& storageManager);

    virtual ~Model();

 protected:
    Storage::Manager& storageManager;
};
}  // namespace Components
