#pragma once

#include "../../default/components/Controller.hpp"
#include "../models/TestModel.hpp"

namespace Components {
class TestController : public Contoller {
 public:
   TestController(TestModel& testModel);

   ~TestController() override;

    View GetHelloMessage();
    View GetAllLoginsOfUsers();
 private:
    TestModel& testModel;
};
}  // namespace Components
