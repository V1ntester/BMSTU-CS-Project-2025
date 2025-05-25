#pragma once

#include <boost/beast/http.hpp>
#include <functional>
#include <string>
#include "../../storage/Manager.hpp"
#include "../components/views/View.hpp"

namespace http = boost::beast::http;

namespace Web {
namespace Routes {

using Function = std::function<Components::View(const boost::beast::http::request<boost::beast::http::string_body>&)>;

class Route {
 public:
    Route(http::verb method, std::string url, Function function);

    virtual ~Route();

    http::verb Method() const;
    std::string Url() const;
    Function Handler() const;

 private:
    http::verb method = http::verb::unknown;
    std::string url = "";
    Function function;
};

class Get : public Route {
 public:
    Get(std::string url, Function function);
};

class Post : public Route {
 public:
    Post(std::string url, Function function);
};

class Put : public Route {
 public:
    Put(std::string url, Function function);
};

class Delete : public Route {
 public:
    Delete(std::string url, Function function);
};
}  // namespace Routes
}  // namespace Web
