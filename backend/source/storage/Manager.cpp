#include "Manager.hpp"

#include <boost/process/detail/traits.hpp>
#include <boost/process/env.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include "../Environment.hpp"

using namespace Storage;

namespace {
const size_t kConnectionsCount = 5;

std::string GetConnectionDataFromEnv() {
    boost::process::environment environment = Environment::Load();

    const std::string kHost = environment["DB_HOST"].to_string();
    const std::string kDataBaseName = environment["DB_NAME"].to_string();
    const std::string kUser = environment["DB_USER"].to_string();
    const std::string kPassword = environment["DB_PASSWORD"].to_string();

    std::string kData = "host=" + kHost + " dbname=" + kDataBaseName + " user=" + kUser + " password=" + kPassword;

    return kData;
}
}  // namespace

Manager::Manager() {
    const std::string kConnectionData = GetConnectionDataFromEnv();

    try {
        for (size_t i = 0; i < kConnectionsCount; i++) {
            this->pool.push(std::make_shared<pqxx::connection>(kConnectionData));
        }
    } catch (const std::exception& exception) {
        std::cerr << "Failed to initialize connection pool: " << exception.what() << '\n';
    }
}

Manager::~Manager() {
    while (!pool.empty()) {
        pool.pop();
    }
}

std::shared_ptr<pqxx::connection> Manager::GetConnection() {
    std::unique_lock<std::mutex> lock(mutex);

    condition.wait(lock, [this] { return !this->pool.empty(); });

    std::shared_ptr<pqxx::connection> connection = this->pool.front();

    this->pool.pop();

    return connection;
}

void Manager::ReturnConnection(std::shared_ptr<pqxx::connection> connection) {
    this->pool.push(connection);
}
