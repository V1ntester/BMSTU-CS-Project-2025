#include "Router.hpp"

#include <iostream>
#include <memory>
#include "../default/web/Routes.hpp"
#include "factories/ResponseFactory.hpp"

#include "../web/Points.cpp"

using namespace Core;

Router::Router() {
    for (std::shared_ptr<Web::Routes::Route>& route : Web::points) {
        std::string url = route->Url();
        Web::Routes::Function function = route->Handler();

        switch (route->Method()) {
            case http::verb::get:
                this->getMethodMap[url] = function;
                break;

            case http::verb::post:
                this->postMethodMap[url] = function;
                break;

            case http::verb::put:
                this->putMethodMap[url] = function;
                break;

            case http::verb::delete_:
                this->deleteMethodMap[url] = function;
                break;

            default:
                break;
        }
    }
}

Router::~Router() = default;

http::response<http::string_body> Router::Route(const http::request<http::string_body>& request) {
    std::string url = request.target();
    http::verb method = request.method();

    Web::Routes::Function function = this->Find(method, url);
    ResponseFactory ResponseFactory;

    http::response<http::string_body> response = ResponseFactory.Make(function(request), request);

    return response;
}

Web::Routes::Function Router::Find(http::verb method, std::string url) {
    switch (method) {
        case http::verb::get:
            if (this->getMethodMap.count(url) > 0) {
                return this->getMethodMap[url];
            }
            break;

        case http::verb::post:
            if (this->postMethodMap.count(url) > 0) {
                return this->postMethodMap[url];
            }
            break;

        case http::verb::put:
            if (this->putMethodMap.count(url) > 0) {
                return this->putMethodMap[url];
            }
            break;

        case http::verb::delete_:
            if (this->deleteMethodMap.count(url) > 0) {
                return this->deleteMethodMap[url];
            }
            break;

        default:
            break;
    }

    return []([[maybe_unused]] const http::request<http::string_body>& request) {
        const std::string message = "Error 404";

        return Components::View(message);
    };
}
