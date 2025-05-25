#pragma once

#include "View.hpp"

namespace Components {
class Error : public View {
    public: 
        Error(size_t kErrorStatusCode);

        ~Error() override;
};
}  // namespace Components
