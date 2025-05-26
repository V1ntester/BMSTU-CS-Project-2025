#pragma once

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "../../storage/Manager.hpp"
#include "views/View.hpp"

namespace Components {

namespace http = boost::beast::http;

using json = nlohmann::json;
using Request = boost::beast::http::request<boost::beast::http::string_body>;

class Controller {
 public:
    Controller();

    virtual ~Controller();
};
}  // namespace Components
