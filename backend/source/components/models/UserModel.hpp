#pragma once

#include <string>

#include "../../default/components/Model.hpp"

namespace Components {
class UserModel : public Model {
 public:
    explicit UserModel(Storage::Manager& storageManager);

    ~UserModel() override;

    bool Identify(std::string login);
    bool Authenticate(std::string login, std::string password);

    void Add(std::string login, std::string email, std::string password);
    void Delete(std::string login);

    void Restore(std::string code, std::string password);
    void Verify(std::string code);

 private:
    std::string GeneratePasswordHash(std::string password);

    std::string GeneratePrimaryVerificationCode();
    std::string GeneratePrimaryUpdatingCode();

    bool CheckIfExistsByLogin(std::string login);
    bool CheckIfExistsByEmail(std::string email);
    bool ComparePasswordForLogin(std::string login, std::string password);
};
}  // namespace Components
