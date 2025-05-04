#include "View.hpp"

using namespace Components;

View::View() {
    this->data = {};
}

View::View(std::string string) {
    this->data["message"] = string;
}

View::View(nlohmann::json data) : data(std::move(data)) {
}

View::~View() = default;

nlohmann::json View::Get() const {
    return this->data;
}
