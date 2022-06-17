#pragma once

#include <vector>
#include "module/user_defined/user_defined_entry.h"

namespace ra::module::user_defined {

std::vector<UserDefinedEntryLegacy> ReadUserDefinedEntries();

}