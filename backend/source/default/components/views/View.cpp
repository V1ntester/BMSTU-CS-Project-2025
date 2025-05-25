#include "View.hpp"

using namespace Components;

View::View() {
    this->data = {};
}

View::View(nlohmann::json data) : data(std::move(data)) {
}

View::View(const View& view) : data(std::move(view.data)) {
}

View::~View() = default;

View& View::operator=(const View& view) {
    this->data = view.data;

    return *this;
}

nlohmann::json View::Get() const {
    return this->data;
}
