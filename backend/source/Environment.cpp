#include "Environment.hpp"

boost::process::environment Environment::Load() {
    boost::process::environment env = boost::this_process::environment();

    std::ifstream file(Environment::kFilePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open environment file\n";
        return env;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t delimeterPosition = line.find("=");

        std::string key = line.substr(0, delimeterPosition);
        std::string value = line.substr(delimeterPosition + 1);

        env[key] = value;
    }

    return env;
}
