#include "../storage/Manager.hpp"
#include "../components/models/TaskModel.hpp"     
#include "../components/controllers/TaskController.hpp" 
#include "../components/models/UserModel.hpp"

namespace Web
{
    Storage::Manager storageManager;

    Components::UserModel userModel(storageManager);
    Components::TaskModel taskModel(storageManager);            
    Components::TaskController taskController(taskModel);       
} // namespace Web