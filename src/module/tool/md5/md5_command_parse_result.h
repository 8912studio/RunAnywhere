#pragma once

#include <optional>
#include <string>
#include "module/active_path/active_path_option.h"
#include "module/common/text_encoding.h"

namespace ra::mod::tool::md5 {

class MD5CommandParseResult {
public:
	std::optional<active_path::ActivePathOption> active_path_option;
	std::wstring string;
	std::optional<TextEncoding> encoding;
	bool treat_string_as_file{};
	bool use_uppercase{};
};

}