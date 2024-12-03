#pragma once

#include <string>
#include <vector>

namespace ra::utility::sql {

struct IndexInfo {
    std::vector<std::string> columns;
};

}