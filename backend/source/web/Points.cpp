#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"
#include "../storage/Manager.hpp"

#include "Components.cpp"

namespace Web {

namespace http = boost::beast::http;

std::shared_ptr<Routes::Route> points[1]{
    std::make_shared<Routes::Post>("/users/auth",
                                   []([[maybe_unused]] const auto& request) -> Components::View { return userController.Authorize(request); }),
};
}  // namespace Web
