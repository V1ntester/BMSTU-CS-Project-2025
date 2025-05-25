#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"
#include "../storage/Manager.hpp"

#include "Components.cpp"

namespace Web {

namespace http = boost::beast::http;

std::shared_ptr<Routes::Route> points[2]{
    std::make_shared<Routes::Get>(
        "/hello", []([[maybe_unused]] const auto& request) -> Components::View { return Web::testController.GetHelloMessage(); }),
    std::make_shared<Routes::Get>(
        "/test", []([[maybe_unused]] const auto& request) -> Components::View { return Web::testController.GetAllLoginsOfUsers(); }),
};
}  // namespace Web
