#pragma once

#include <boost/process/environment.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace Environment {
const std::string kFilePath = ".env";

boost::process::environment LoadEnvironment();
}  // namespace Environment
