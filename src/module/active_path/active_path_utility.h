#pragma once

#include <filesystem>
#include "module/active_path/active_path_option.h"

namespace ra::module::active_path {

std::wstring AdjustActivePathByOption(
	const std::filesystem::path& active_path,
	const ActivePathOption& option);

}