#pragma once

#include <optional>
#include <string>
#include "module/active_path/active_path_option.h"
#include "module/common/general_option.h"
#include "module/common/text_encoding.h"

namespace ra::mod::tool::md5 {

class MD5CommandParseResult {
public:
	GeneralOption general_option;
	bool use_uppercase{};
};

}