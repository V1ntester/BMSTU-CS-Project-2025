#include "UserController.hpp"

#include <boost/algorithm/string.hpp>
#include <map>
#include <string>
#include <vector>
#include "pqxx/pqxx"

using namespace Components;

namespace {
const size_t kOKStatusCode = 200;
const size_t kBadRequestStatusCode = 400;
const size_t kForbiddenStatusCode = 403;
}  // namespace

UserController::UserController(UserModel& userModel) : userModel(userModel) {
}

UserController::~UserController() = default;

View UserController::Authorize(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (const std::exception& exception) {
        return {answer};
    }

    if (!requestBody.contains("login") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string password = requestBody["password"];

    // Вот должна быть проверка этих строк, нормальные ли они (Длина, trim и прочее)

    bool isIndentify = this->userModel.Identify(login);

    if (isIndentify) {
        bool isAuthenticate = this->userModel.Authenticate(login, password);

        if (isAuthenticate) {
            answer["code"] = kOKStatusCode;
            answer["message"] = "OK";

            return answer;
        }
    }

    answer["code"] = kForbiddenStatusCode;
    answer["message"] = "Forbidden";

    return {answer};
}

// View UserController::Add(const Request& request) {
// }
// View UserController::Delete(const Request& request) {
// }
// View UserController::Restore(const Request& request) {
// }
// View UserController::Verify(const Request& request) {
// }
