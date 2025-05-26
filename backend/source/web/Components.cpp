#include "../storage/Manager.hpp"

#include "../components/models/TaskModel.hpp"     
#include "../components/controllers/TaskController.hpp" 

namespace Web
{
    Storage::Manager storageManager;

    Components::TaskModel taskModel(storageManager);            
    Components::TaskController taskController(taskModel);       
} // namespace Web
