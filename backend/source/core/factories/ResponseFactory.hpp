#include <boost/beast/http.hpp>
#include <string>
#include "../../default/components/views/View.hpp"

namespace Core {
namespace http = boost::beast::http;

class ResponseFactory {
 public:
    ResponseFactory();

    ~ResponseFactory();

    http::response<http::string_body> Make(Components::View view, http::request<http::string_body> request);
};
}  // namespace Core
