#include "ResponseFactory.hpp"

using namespace Core;

ResponseFactory::ResponseFactory() = default;

ResponseFactory::~ResponseFactory() = default;

http::response<http::string_body> ResponseFactory::Make(Components::View view, http::request<http::string_body> request) {
    http::response<http::string_body> response;

    response.set(http::field::server, "BMSTU-project");
    response.set(http::field::content_type, "application/json");
    response.keep_alive(request.keep_alive());
    response.body() = view.Get().dump();
    response.prepare_payload();

    return response;
}
