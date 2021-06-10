#pragma once

#include <string>
#include "module/crypto/md5/md5_encoding.h"

namespace ra::module::crypto {

class MD5CommandParseResult {
public:
	std::wstring string;
	MD5Encoding encoding{ MD5Encoding::UTF8 };
	bool use_uppercase{};
};

}