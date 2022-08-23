#pragma once

#include <filesystem>
#include <vector>
#include <module/user_defined/legacy/legacy_entry.h>

namespace ra::module::user_defined{

std::vector<LegacyEntry> ReadUserDefinedEntries(const std::filesystem::path& file_path);

}