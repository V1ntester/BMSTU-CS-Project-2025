#include "TestModel.hpp"

#include <pqxx/pqxx>

using namespace Components;

TestModel::TestModel(Storage::Manager& storageManager) : Model(storageManager) {
}

TestModel::~TestModel() = default;

std::vector<std::string> TestModel::GetAllLoginsOfUsersInDataBase() {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    pqxx::result result = transaction.exec("SELECT * FROM \"Users\"");

    transaction.commit();

    std::vector<std::string> logins;

    for (auto row : result) {
        logins.push_back(row["login"].as<std::string>());
    }

    return logins;
}
