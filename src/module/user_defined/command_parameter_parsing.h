#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

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
	std::size_t backward_level{};
	bool is_quoted{};
};


std::vector<CommandParameter> ParseParameter(std::wstring_view parameter);

}