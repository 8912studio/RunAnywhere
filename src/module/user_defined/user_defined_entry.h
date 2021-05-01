#pragma once

#include <string>

namespace ra::module::user_defined {

class UserDefinedEntry {
public:
    std::wstring keyword;
    std::wstring command_line;
};

}