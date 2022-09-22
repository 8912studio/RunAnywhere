#pragma once

#include "module/user_defined/command_line_info.h"

namespace ra::module::user_defined {

class ExecuteInfo {
public:
    CommandLineInfo command_line;
    std::wstring working_directory;
};

}