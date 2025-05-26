#include "../storage/Manager.hpp"

<<<<<<< HEAD
#include "../components/models/UserModel.hpp"
#include "../components/controllers/UserController.hpp"
=======
#include "../components/models/TaskModel.hpp"     
#include "../components/controllers/TaskController.hpp" 
>>>>>>> feature/tasks-functions-init

namespace Web
{
    Storage::Manager storageManager;

<<<<<<< HEAD
    Components::UserModel userModel(storageManager);
    Components::UserController userController(userModel);
} // namespace Web
=======
    Components::TaskModel taskModel(storageManager);            
    Components::TaskController taskController(taskModel);       
} // namespace Web
>>>>>>> feature/tasks-functions-init
