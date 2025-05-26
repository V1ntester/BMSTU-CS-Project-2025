#include "UserController.hpp"

#include <boost/url.hpp>
#include <string>
#include "pqxx/pqxx"

using namespace Components;
namespace {
const size_t kOKStatusCode = 200;
const size_t kForbiddenStatusCode = 403;
}

UserController::UserController(UserModel& userModel) : userModel(userModel) {
}

UserController::~UserController() = default;

View UserController::Authorize(const Request& request) {
    boost::urls::params_encoded_view params = boost::urls::parse_query(request.body()).value();

    json answer;

    bool isIndentify = this->userModel.Identify(std::string((*(params.find("login"))).value));

    if (isIndentify) {
        bool isAuthenticate =
            this->userModel.Authenticate(std::string((*(params.find("login"))).value), std::string((*(params.find("password"))).value));

        if (isAuthenticate) {
            answer["code"] = kOKStatusCode;
            answer["message"] = "OK";

            return answer;
        }
    }

    answer["code"] = kForbiddenStatusCode;
    answer["message"] = "Forbidden";

    return answer;
}

// View UserController::Add(const Request& request) {
// }
// View UserController::Delete(const Request& request) {
// }
// View UserController::Restore(const Request& request) {
// }
// View UserController::Verify(const Request& request) {
// }
