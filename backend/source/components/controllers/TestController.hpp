#pragma once

#include "../../default/components/Controller.hpp"
#include "../../storage/Manager.hpp"

namespace Components {
// Bad Practice
class TestController : public Contoller {
 public:
    View Do(Storage::Manager& manager);
};
}  // namespace Components
