#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace Components {
class View {
 public:
    View();
    View(std::string string);
    View(nlohmann::json data);

    ~View();

    nlohmann::json Get() const;

 private:
    nlohmann::json data;
};
}  // namespace Components
