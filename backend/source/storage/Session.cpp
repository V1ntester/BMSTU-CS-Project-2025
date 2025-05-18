#include "Manager.hpp"

using namespace Storage;

Session::Session(Manager& manager) : manager(manager) {
    this->connection = this->manager.GetConnection();
}

Session::~Session() {
    this->manager.ReturnConnection(this->connection);
}

pqxx::connection& Session::Get() const {
    return *(this->connection);
}
