#pragma once

#include <string>

#include "../../default/components/Model.hpp"

namespace Components {
class UserModel : public Model {
 public:
    explicit UserModel(Storage::Manager& storageManager);

    ~UserModel() override;

    bool IdentifyByLogin(std::string login) const;
    bool IdentifyByEmail(std::string email) const;

    bool Authenticate(std::string login, std::string password) const;

    void Add(std::string login, std::string email, std::string password);
    void Delete(std::string login);

    void Restore(std::string code, std::string password);
    void Verify(std::string code);

 private:
    std::string GeneratePasswordHash(std::string password) const;

    std::string GeneratePrimaryVerificationCode() const;
    std::string GeneratePrimaryUpdatingCode() const;

    bool CheckIfExistsByLogin(std::string login) const;
    bool CheckIfExistsByEmail(std::string email) const;
    bool ComparePasswordForLogin(std::string login, std::string password) const;
};
}  // namespace Components
