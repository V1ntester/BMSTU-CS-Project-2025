#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <pqxx/pqxx>
#include <queue>

namespace Storage {
class Manager;

class Session {
 public:
    explicit Session(Manager& manager);
    Session(const Session& session) = delete;

    ~Session();

    Session& operator=(const Session&) = delete;

    pqxx::connection& Get() const;

 private:
    std::shared_ptr<pqxx::connection> connection;

    Manager& manager;
};

class Manager {
 public:
    Manager();

    ~Manager();

 private:
    std::shared_ptr<pqxx::connection> GetConnection();
    void ReturnConnection(std::shared_ptr<pqxx::connection> connection);

    std::mutex mutex;
    std::condition_variable condition;
    std::queue<std::shared_ptr<pqxx::connection>> pool;

    friend Session;
};
}  // namespace Storage
