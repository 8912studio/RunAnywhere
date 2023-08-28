#pragma once

#include <optional>
#include <string>
#include "module/active_path/active_path_option.h"
#include "module/common/general_option.h"
#include "module/common/text_encoding.h"

namespace ra::mod::tool::hash {

class HashCommandParseResult {
public:
	GeneralOption general_option;
	bool use_uppercase{};
};

}