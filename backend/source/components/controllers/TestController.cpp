#include "TestController.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include "pqxx/pqxx"

using namespace Components;
using json = nlohmann::json;

// Bad Practice
View TestController::Do(Storage::Manager& manager) {
    Storage::Session session(manager);

    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT * FROM \"Users\"");

    transaction.commit();

    json logins = json::array();

    for (auto row : result) {
        logins.push_back(row["login"].as<std::string>());
    }

    return {logins};
}
