#pragma once

#include "../../default/components/Controller.hpp"
#include "../models/UserModel.hpp"

namespace Components {
class UserController : public Controller {
 public:
    explicit UserController(UserModel& userModel);

    ~UserController() override;

    View Authorize(const Request& request);

    View Add(const Request& request);
    View Delete(const Request& request);

    View Restore(const Request& request);
    View Verify(const Request& request);

 private:
    UserModel& userModel;
};
}  // namespace Components
