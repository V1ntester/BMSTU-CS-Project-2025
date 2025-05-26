// For UserModel::GeneratePasswordHash Function
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "UserModel.hpp"

#include <openssl/sha.h>
#include <boost/algorithm/hex.hpp>
#include <pqxx/pqxx>
#include <random>

using namespace Components;

namespace {
const size_t kVerificationCodeLength = 64;
const size_t kUpdatingCodeLength = 64;

std::string GenerateRandomString(size_t length) {
    const std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, chars.size() - 1);

    std::string string;

    for (size_t i = 0; i < length; ++i) {
        string += chars[distribution(generator)];
    }

    return string;
}
}  // namespace

UserModel::UserModel(Storage::Manager& storageManager) : Model(storageManager) {
}

UserModel::~UserModel() = default;

bool UserModel::Identify(std::string login) {
    return this->CheckIfExistsByLogin(login);
}

bool UserModel::Authenticate(std::string login, std::string password) {
    return this->ComparePasswordForLogin(login, password);
}

void UserModel::Add(std::string login, std::string email, std::string password) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    std::string passwordHash = this->GeneratePasswordHash(password);

    std::string verificationCode = this->GeneratePrimaryVerificationCode();
    std::string updatingCode = this->GeneratePrimaryUpdatingCode();

    transaction.exec(
        "INSERT INTO \"Users\" (login, email, password_hash, verification_code, updating_code, created_at) VALUES ($1, $2, $3, $4, $5, "
        "CURRENT_TIMESTAMP)",
        pqxx::params{login, email, passwordHash, verificationCode, updatingCode});

    transaction.commit();
}

void UserModel::Delete(std::string login) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    transaction.exec("DELETE FROM \"Users\" WHERE login = $1", pqxx::params{login});

    transaction.commit();
}

void UserModel::Restore(std::string code, std::string password) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    std::string passwordHash = this->GeneratePasswordHash(password);

    transaction.exec("UPDATE \"Users\" SET password_hash = $2 WHERE verification_code = $1", pqxx::params{code, passwordHash});

    transaction.commit();
}

void UserModel::Verify(std::string code) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    transaction.exec("UPDATE \"Users\" SET verified = true WHERE verification_code = $1", pqxx::params{code});

    transaction.commit();
}

std::string UserModel::GeneratePasswordHash(std::string password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);

    std::string hexHash;

    boost::algorithm::hex(hash, hash + SHA256_DIGEST_LENGTH, std::back_inserter(hexHash));

    return hexHash;
}

std::string UserModel::GeneratePrimaryVerificationCode() {
    std::string code;

    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    size_t count = 1;

    while (count > 0) {
        code = GenerateRandomString(kVerificationCodeLength);

        pqxx::work transaction(session.Get());

        count = transaction.query_value<size_t>("SELECT COUNT(*) FROM \"Users\" WHERE verification_code = " + code);

        transaction.commit();
    }

    return code;
}

std::string UserModel::GeneratePrimaryUpdatingCode() {
    std::string code;

    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    size_t count = 1;

    while (count > 0) {
        code = GenerateRandomString(kUpdatingCodeLength);

        pqxx::work transaction(session.Get());

        count = transaction.query_value<size_t>("SELECT COUNT(*) FROM \"Users\" WHERE updating_code = " + code);

        transaction.commit();
    }

    return code;
}

bool UserModel::CheckIfExistsByLogin(std::string login) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    size_t count = transaction.query_value<size_t>("SELECT COUNT(*) FROM \"Users\" WHERE login = $1", pqxx::params(login));

    transaction.commit();

    return count > 0;
}

bool UserModel::CheckIfExistsByEmail(std::string email) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    size_t count = transaction.query_value<size_t>("SELECT COUNT(*) FROM \"Users\" WHERE email = $1", pqxx::params(email));

    transaction.commit();

    return count > 0;
}

bool UserModel::ComparePasswordForLogin(std::string login, std::string password) {
    Storage::Session session(this->storageManager);

    pqxx::work transaction(session.Get());

    std::string passwordHashOrigin =
        transaction.query_value<std::string>("SELECT password_hash FROM \"Users\" WHERE login = $1", pqxx::params(login));

    transaction.commit();

    if (this->GeneratePasswordHash(password) == passwordHashOrigin) {
        return true;
    } else {
        return false;
    }
}
