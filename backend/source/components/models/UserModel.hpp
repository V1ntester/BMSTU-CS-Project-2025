#pragma once
// это
#include <string>
#include <vector>
#include "../../default/components/Model.hpp"

namespace Components {

class UserModel : public Model {
 public:
    explicit UserModel(Storage::Manager& storageManager);
    ~UserModel() = default;

    struct User {
        int id;
        std::string login;
        std::string email;
        std::string name;
    };

    bool registerUser(const std::string& login, const std::string& password, const std::string& email, const std::string& name);

    bool authenticateUser(const std::string& login, const std::string& password);

    User getUserByLogin(const std::string& login);
    User getUserById(int user_id);

 private:
    std::string hashPassword(const std::string& password) const;
    void validateUserData(const std::string& login, const std::string& email) const;
};

}  // namespace Components
