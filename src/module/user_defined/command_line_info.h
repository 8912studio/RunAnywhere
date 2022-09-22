#pragma once

#include <string>
#include <vector>

namespace ra::module::user_defined {

class CommandLineInfo {
public:
    std::wstring command;
    std::vector<std::wstring> arguments;
};

}