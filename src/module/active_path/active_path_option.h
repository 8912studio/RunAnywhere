#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace ra::mod::active_path {

class ActivePathOption {
public:
	std::optional<std::wstring> overriding_path;
	std::size_t backward_level{};
	bool use_workspace_path{};
	bool use_name{};
};

}