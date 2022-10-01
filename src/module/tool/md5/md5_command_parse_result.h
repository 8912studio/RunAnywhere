#pragma once

#include <string>
#include "module/tool/md5/md5_encoding.h"

namespace ra::module::tool::md5 {

class MD5CommandParseResult {
public:
	std::wstring string;
	MD5Encoding encoding{ MD5Encoding::UTF8 };
	bool use_uppercase{};
};

}