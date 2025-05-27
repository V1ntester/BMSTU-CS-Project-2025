#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"
#include "../storage/Manager.hpp"

#include "Components.cpp"

namespace Web {

namespace http = boost::beast::http;

std::shared_ptr<Routes::Route> points[5]{
    std::make_shared<Routes::Get>("/user",
                                  []([[maybe_unused]] const auto& request) -> Components::View { return Web::userController.Authorize(request); }),
    std::make_shared<Routes::Post>("/user",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return Web::userController.Add(request); }),
    std::make_shared<Routes::Put>("/user",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return Web::userController.Restore(request); }),
    std::make_shared<Routes::Delete>("/user",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return Web::userController.Delete(request); }),
    std::make_shared<Routes::Post>("/user/verify",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return Web::userController.Verify(request); }),
    // std::make_shared<Routes::Post>(
    //     "/tasks/create", [](const auto& request) -> Components::View {
    //         return Web::taskController.CreateTask();
    //     }),

    // std::make_shared<Routes::Put>(
    //     "/tasks/update", [](const auto& request) -> Components::View {
    //         return Web::taskController.UpdateTask();
    //     }),

    // std::make_shared<Routes::Delete>(
    //     "/tasks/delete", [](const auto& request) -> Components::View {
    //         return Web::taskController.DeleteTask();
    //     }),

    // std::make_shared<Routes::Get>(
    //     "/tasks", [](const auto& request) -> Components::View {
    //         return Web::taskController.GetAllTasks();
    //     }),

    // std::make_shared<Routes::Get>(
    //     "/tasks/priority", [](const auto& request) -> Components::View {
    //         return Web::taskController.GetTasksByPriority();
    //     }),

    // std::make_shared<Routes::Get>(
    //     "/tasks/category", [](const auto& request) -> Components::View {
    //         return Web::taskController.GetTasksByCategory();
    //     })
};
}  // namespace Web
