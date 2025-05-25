#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace Components {
class View {
 public:
    View();
    View(nlohmann::json data);
    View(const View& view);

    virtual ~View();

    View& operator=(const View& view);

    nlohmann::json Get() const;

 protected:
    nlohmann::json data;
};
}  // namespace Components
