#pragma once

#include <filesystem>
#include <vector>
#include <module/user_defined/legacy/legacy_entry.h>

namespace ra::mod::user_defined{

std::vector<LegacyEntry> ReadUserDefinedEntries(const std::filesystem::path& file_path);

}