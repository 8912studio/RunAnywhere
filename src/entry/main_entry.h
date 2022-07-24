#pragma once

#include <string>
#include "entry/entry_result.h"

namespace ra::entry {

EntryResult RunMainEntry(const std::wstring& command_line);

}