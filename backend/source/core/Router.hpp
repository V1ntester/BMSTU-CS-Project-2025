#pragma once

#include <boost/beast/http.hpp>
#include <unordered_map>
#include <memory>
#include <string>

#include "../default/web/Routes.hpp"

namespace Core {
namespace http = boost::beast::http;

class Router {
 public:
    Router();

    ~Router();

    http::response<http::string_body> Route(const http::request<http::string_body>& request);

 private:
    std::unordered_map<std::string, Web::Routes::Function> getMethodMap;
    std::unordered_map<std::string, Web::Routes::Function> postMethodMap;
    std::unordered_map<std::string, Web::Routes::Function> putMethodMap;
    std::unordered_map<std::string, Web::Routes::Function> deleteMethodMap;

    Web::Routes::Function Find(http::verb method, std::string url);
};
}  // namespace Core
