#include "utility/sql/orm/data_set_helpers.h"
#include <zaf/base/range.h>

namespace ra::utility::sql {

std::string JoinPlaceholders(std::size_t count) {

    std::string result;
    for (auto index : zaf::Range{ 0, count }) {
        if (index != 0) {
            result += ',';
        }
        result += '?';
    }
    return result;
}

}