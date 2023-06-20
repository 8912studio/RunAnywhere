#pragma once

#include <string>
#include "module/common/text_encoding.h"

namespace ra::mod::tool::md5 {

class MD5CommandParseResult {
public:
	std::wstring string;
	TextEncoding encoding{ TextEncoding::UTF8 };
	bool use_uppercase{};
};

}