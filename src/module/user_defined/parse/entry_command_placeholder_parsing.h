#pragma once

#include <cstddef>
#include <string_view>
#include <vector>
#include "module/active_path/active_path_option.h"

namespace ra::module::user_defined {

class EntryCommandPlaceholder {
public:
	std::size_t position{};
	std::size_t length{};

	std::size_t index{};
};


std::vector<EntryCommandPlaceholder> ParseEntryCommandPlaceholders(std::wstring_view input);

}