#pragma once

#include <ctime>
#include <optional>

namespace ra::mod::tool::date {

class DateCommandParseResult {
public:
	std::optional<std::time_t> value;
	bool output_raw_value{};
};

}