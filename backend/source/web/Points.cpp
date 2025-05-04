#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"

namespace Web {

namespace http = boost::beast::http;

std::shared_ptr<Routes::Route> points[]{
    std::make_shared<Routes::Get>(
        "/hello",
        [](const http::request<http::string_body>&) -> Components::View {
            return Components::View(std::string("Hello World!"));
        }
    ),
};
}  // namespace Web
