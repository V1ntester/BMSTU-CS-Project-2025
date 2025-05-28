#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"
#include "../storage/Manager.hpp"

#include "Components.cpp"

namespace Web {

namespace http = boost::beast::http;

std::shared_ptr<Routes::Route> points[11]{
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
    // Task routes
    std::make_shared<Routes::Get>("/tasks",
                                  []([[maybe_unused]] const auto& request) -> Components::View { return Web::taskController.Get(request); }),
    std::make_shared<Routes::Post>("/task",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return Web::taskController.Add(request); }),
    std::make_shared<Routes::Put>("/task",
                                  []([[maybe_unused]] const auto& request) -> Components::View { return Web::taskController.Update(request); }),
    std::make_shared<Routes::Delete>("/task",
                                     []([[maybe_unused]] const auto& request) -> Components::View { return Web::taskController.Delete(request); }),
    std::make_shared<Routes::Get>("/tasks/priority",
                                  []([[maybe_unused]] const auto& request) -> Components::View { return Web::taskController.GetByPriority(request); }),
    std::make_shared<Routes::Get>("/tasks/category",
                                  []([[maybe_unused]] const auto& request) -> Components::View { return Web::taskController.GetByCategory(request); })
};
}  // namespace Web
