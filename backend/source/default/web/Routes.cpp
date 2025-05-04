#include "Routes.hpp"

using namespace Web::Routes;

Route::Route(http::verb method, std::string url, Function function) : method(method), url(std::move(url)), function(std::move(function)) {
}

Route::~Route() = default;

http::verb Route::Method() const {
    return this->method;
}
std::string Route::Url() const {
    return this->url;
}

Function Route::Handler() const {
    return this->function;
}

Get::Get(std::string url, Function function) : Route(http::verb::get, url, function) {
}

Post::Post(std::string url, Function function) : Route(http::verb::post, url, function) {
}

Put::Put(std::string url, Function function) : Route(http::verb::put, url, function) {
}

Delete::Delete(std::string url, Function function) : Route(http::verb::delete_, url, function) {
}
