#pragma once

#include <ctime>
#include <optional>
#include <vector>
#include "module/tool/date/date_time_unit.h"

namespace ra::mod::tool::date {

class DateTimeAdjustment {
public:
	int adjustment{};
	DateTimeUnit unit{ DateTimeUnit::Second };
};

class DateCommandParseResult {
public:
	std::optional<std::time_t> value;
	std::vector<DateTimeAdjustment> adjustments;
	bool output_raw_value{};
};

}