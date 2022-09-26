#pragma once

#include <string>
#include <vector>

namespace ra::module::user_defined {

class CommandLineInfo {
public:
    std::wstring command;

    //Each argument is enclosed with double quotes if it has space.
    std::vector<std::wstring> arguments;
};

}