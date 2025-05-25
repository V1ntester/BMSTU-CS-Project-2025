#include "../storage/Manager.hpp"

#include "../components/models/TestModel.hpp"
#include "../components/controllers/TestController.hpp"

namespace Web
{
    Storage::Manager storageManager;

    Components::TestModel testModel(storageManager);
    Components::TestController testController(testModel);
} // namespace Web
