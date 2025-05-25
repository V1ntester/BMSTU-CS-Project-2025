#include "TestController.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include "pqxx/pqxx"

using namespace Components;
using json = nlohmann::json;

namespace {
const size_t kSuccessStatusCode = 200;
}

TestController::TestController(TestModel& testModel) : testModel(testModel) {
}

TestController::~TestController() = default;

View TestController::GetHelloMessage() {
    json answer;

    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Hello!";

    return {answer};
}

View TestController::GetAllLoginsOfUsers() {
    json answer;

    std::vector<std::string> logins = this->testModel.GetAllLoginsOfUsersInDataBase();

    answer["result"] = logins;

    answer["code"] = kSuccessStatusCode;
    answer["message"] = "Success";

    return {answer};
}
