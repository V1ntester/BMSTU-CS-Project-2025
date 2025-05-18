#include <boost/beast/http.hpp>
#include <memory>
#include "../default/components/views/View.hpp"
#include "../default/web/Routes.hpp"
#include "../storage/Manager.hpp"

#include "../components/controllers/TestController.hpp"

namespace Web {

namespace http = boost::beast::http;

Components::TestController testController;

std::shared_ptr<Routes::Route> points[2]{
    std::make_shared<Routes::Get>("/hello",
                                  []([[maybe_unused]] const auto& request, [[maybe_unused]] auto& manager) -> Components::View {
                                      return Components::View(std::string("Hello World!"));
                                  }),
    std::make_shared<Routes::Get>(
        "/test",
        []([[maybe_unused]] const auto& request, [[maybe_unused]] auto& manager) -> Components::View { return Web::testController.Do(manager); }),
};
}  // namespace Web
