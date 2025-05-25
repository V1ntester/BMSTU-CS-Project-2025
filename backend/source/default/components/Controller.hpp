#pragma once

#include <boost/beast/http.hpp>
#include "../../storage/Manager.hpp"
#include "views/View.hpp"

namespace Components {

namespace http = boost::beast::http;

class Contoller {
 public:
    Contoller();

    virtual ~Contoller();
};
}  // namespace Components
