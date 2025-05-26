#include "../storage/Manager.hpp"

#include "../components/models/UserModel.hpp"
#include "../components/controllers/UserController.hpp"

namespace Web
{
    Storage::Manager storageManager;

    Components::UserModel userModel(storageManager);
    Components::UserController userController(userModel);
} // namespace Web
