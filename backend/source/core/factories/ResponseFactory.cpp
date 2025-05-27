#include "ResponseFactory.hpp"

#include <nlohmann/json.hpp>

using namespace Core;
using json = nlohmann::json;

ResponseFactory::ResponseFactory() = default;

ResponseFactory::~ResponseFactory() = default;

http::response<http::string_body> ResponseFactory::Make(Components::View view, http::request<http::string_body> request) {
    http::response<http::string_body> response;

    response.set(http::field::server, "BMSTU-project");
    response.set(http::field::content_type, "application/json");

    json data = view.Get();

    if (data.contains("code")) {
        response.result(static_cast<size_t>(data["code"]));
    }

    response.keep_alive(request.keep_alive());
    response.body() = data.dump();
    response.prepare_payload();

    return response;
}
