#pragma once

#include <cstddef>
#include <string_view>
#include <vector>
#include "module/active_path/active_path_option.h"

namespace ra::module::user_defined {

class CommandParameter {
public:
	enum class Type {
		Unknown,
		General,
		ActivePath,
	};

public:
	Type type{ Type::Unknown };

	std::size_t position{};
	std::size_t length{};

	std::size_t general_index{};
	bool is_quoted{};

	active_path::ActivePathOption active_path_option;
};


std::vector<CommandParameter> ParseParameter(std::wstring_view parameter);

}