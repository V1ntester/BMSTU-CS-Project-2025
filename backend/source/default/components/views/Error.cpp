#include "Error.hpp"

using namespace Components;

Error::~Error() = default;

Error::Error(size_t kErrorStatusCode) {
    this->data["code"] = kErrorStatusCode;
    this->data["message"] = "Error without description";
}
