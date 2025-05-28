#include "UserController.hpp"

#include <boost/algorithm/string.hpp>
#include <map>
#include <string>
#include <vector>
#include "pqxx/pqxx"

using namespace Components;

namespace {
// const size_t kOKStatusCode = 200;
const size_t kCreatedStatusCode = 201;
const size_t kAcceptedStatusCode = 202;
const size_t kBadRequestStatusCode = 400;
const size_t kForbiddenStatusCode = 403;
const size_t kNotAcceptableStatusCode = 406;
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

    bool isIndentify = this->userModel.IdentifyByLogin(login);

    if (isIndentify) {
        bool isAuthenticate = this->userModel.Authenticate(login, password);

        if (isAuthenticate) {
            answer["code"] = kAcceptedStatusCode;
            answer["message"] = "Accepted";

            return answer;
        }
    }

    answer["code"] = kForbiddenStatusCode;
    answer["message"] = "Forbidden";

    return {answer};
}

View UserController::Add(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (const std::exception& exception) {
        return {answer};
    }

    if (!requestBody.contains("login") || !requestBody.contains("email") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string login = requestBody["login"];
    std::string email = requestBody["email"];
    std::string password = requestBody["password"];

    bool isIndentify = this->userModel.IdentifyByLogin(login) || this->userModel.IdentifyByEmail(email);

    if (!isIndentify) {
        this->userModel.Add(login, email, password);

        answer["code"] = kCreatedStatusCode;
        answer["message"] = "Created";
    } else {
        answer["code"] = kNotAcceptableStatusCode;
        answer["message"] = "Not Acceptable";
    }

    return answer;
}

View UserController::Delete(const Request& request) {
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

    bool isIndentify = this->userModel.IdentifyByLogin(login);

    if (isIndentify) {
        bool isAuthenticate = this->userModel.Authenticate(login, password);

        if (isAuthenticate) {
            userModel.Delete(login);

            answer["code"] = kAcceptedStatusCode;
            answer["message"] = "Accepted";

            return answer;
        }
    }

    answer["code"] = kForbiddenStatusCode;
    answer["message"] = "Forbidden";

    return {answer};
}

View UserController::Restore(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (const std::exception& exception) {
        return {answer};
    }

    if (!requestBody.contains("code") || !requestBody.contains("password")) {
        return {answer};
    }

    std::string code = requestBody["code"];
    std::string password = requestBody["password"];

    this->userModel.Restore(code, password);

    answer["code"] = kAcceptedStatusCode;
    answer["message"] = "Accepted";

    return answer;
}

View UserController::Verify(const Request& request) {
    json answer;
    json requestBody;

    answer["code"] = kBadRequestStatusCode;
    answer["message"] = "Bad Request";

    try {
        requestBody = json::parse(request.body());
    } catch (const std::exception& exception) {
        return {answer};
    }

    if (!requestBody.contains("code")) {
        return {answer};
    }

    std::string code = requestBody["code"];

    this->userModel.Verify(code);

    answer["code"] = kAcceptedStatusCode;
    answer["message"] = "Accepted";

    return answer;
}
