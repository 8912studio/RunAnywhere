#pragma once

#include <cstddef>

namespace ra::module::active_path {

class ActivePathOption {
public:
	std::size_t backward_level{};
	bool name_only{};
};

}