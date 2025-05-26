#include "UserModel.hpp"
#include <functional>
#include <pqxx/pqxx>
#include <regex>
#include <stdexcept>
// это
using namespace Components;

UserModel::UserModel(Storage::Manager& storageManager) : Model(storageManager) {
}

std::string UserModel::hashPassword(const std::string& password) const {
    const std::string salt = "BMSTU_CS_PROJECT_2025_SALT";
    size_t combined_hash = std::hash<std::string>{}(password + salt);
    return std::to_string(combined_hash);
}

void UserModel::validateUserData(const std::string& login, const std::string& email) const {
    if (login.empty() || email.empty()) {
        throw std::invalid_argument("Login and email cannot be empty");
    }

    static const std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!std::regex_match(email, email_regex)) {
        throw std::invalid_argument("Invalid email format");
    }
}

bool UserModel::registerUser(const std::string& login, const std::string& password, const std::string& email, const std::string& name) {
    validateUserData(login, email);

    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        const std::string check_query = "SELECT id FROM \"Users\" WHERE login = " + transaction.quote(login);
        pqxx::result result = transaction.exec(check_query);

        if (!result.empty()) {
            return false;
        }

        std::string hashed_password = hashPassword(password);

        const std::string insert_query =
            "INSERT INTO \"Users\" (login, password_hash, email, name) "
            "VALUES (" +
            transaction.quote(login) + ", " + transaction.quote(hashed_password) + ", " + transaction.quote(email) + ", " + transaction.quote(name) +
            ") RETURNING id";

        pqxx::result insert_result = transaction.exec(insert_query);
        transaction.commit();
        return !insert_result.empty();
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Registration failed: " + std::string(e.what()));
    }
}

bool UserModel::authenticateUser(const std::string& login, const std::string& password) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    try {
        const std::string query =
            "SELECT password_hash FROM \"Users\" "
            "WHERE login = " +
            transaction.quote(login);

        pqxx::result result = transaction.exec(query);

        if (result.empty()) {
            return false;
        }

        std::string stored_hash = result[0]["password_hash"].as<std::string>();
        std::string input_hash = hashPassword(password);
        return input_hash == stored_hash;
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Authentication failed: " + std::string(e.what()));
    }
}

UserModel::User UserModel::getUserByLogin(const std::string& login) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    User user;
    try {
        const std::string query =
            "SELECT id, login, email, name FROM \"Users\" "
            "WHERE login = " +
            transaction.quote(login);

        pqxx::result result = transaction.exec(query);

        if (!result.empty()) {
            user.id = result[0]["id"].as<int>();
            user.login = result[0]["login"].as<std::string>();
            user.email = result[0]["email"].as<std::string>();
            user.name = result[0]["name"].as<std::string>();
        }
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Failed to get user: " + std::string(e.what()));
    }

    return user;
}

UserModel::User UserModel::getUserById(int user_id) {
    Storage::Session session(this->storageManager);
    pqxx::work transaction(session.Get());

    User user;
    try {
        const std::string query =
            "SELECT id, login, email, name FROM \"Users\" "
            "WHERE id = " +
            std::to_string(user_id);

        pqxx::result result = transaction.exec(query);

        if (!result.empty()) {
            user.id = result[0]["id"].as<int>();
            user.login = result[0]["login"].as<std::string>();
            user.email = result[0]["email"].as<std::string>();
            user.name = result[0]["name"].as<std::string>();
        }
    } catch (const std::exception& e) {
        transaction.abort();
        throw std::runtime_error("Failed to get user: " + std::string(e.what()));
    }

    return user;
}
