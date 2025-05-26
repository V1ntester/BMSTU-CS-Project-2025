#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"
#include "../storage/Manager.hpp"

#include "Components.cpp"

namespace Web {

namespace http = boost::beast::http;

<<<<<<< HEAD
std::shared_ptr<Routes::Route> points[1]{
    std::make_shared<Routes::Post>("/users/auth",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return userController.Authorize(request); }),
=======
std::shared_ptr<Routes::Route> points[6]{

    std::make_shared<Routes::Post>(
        "/tasks/create", [](const auto& request) -> Components::View {
            return Web::taskController.CreateTask(json::parse(request.body()));
        }),
    
    std::make_shared<Routes::Put>(
        "/tasks/update", [](const auto& request) -> Components::View {
            return Web::taskController.UpdateTask(json::parse(request.body()));
        }),
    
    std::make_shared<Routes::Delete>(
        "/tasks/delete", [](const auto& request) -> Components::View {
            int taskId = extractIdFromPath(request.target().to_string());
            int userId = 1; // TODO: Получить из аутентификации
            return Web::taskController.DeleteTask(taskId, userId);
        }),
    
    std::make_shared<Routes::Get>(
        "/tasks", []([[maybe_unused]] const auto& request) -> Components::View {
            int userId = 1; // TODO: Получить из аутентификации
            return Web::taskController.GetAllTasks(userId);
        }),
    
    std::make_shared<Routes::Get>(
        "/tasks/priority", [](const auto& request) -> Components::View {
            int priority = extractIdFromPath(request.target().to_string());
            int userId = 1; // TODO: Получить из аутентификации
            return Web::taskController.GetTasksByPriority(priority, userId);
        }),
    
    std::make_shared<Routes::Get>(
        "/tasks/category", [](const auto& request) -> Components::View {
            int category = extractIdFromPath(request.target().to_string());
            int userId = 1; // TODO: Получить из аутентификации
            return Web::taskController.GetTasksByCategory(category, userId);
        })
>>>>>>> feature/tasks-functions-init
};
} // namespace Web

