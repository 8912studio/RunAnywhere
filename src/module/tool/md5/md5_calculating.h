#pragma once

#include <filesystem>
#include <string>
#include <zaf/rx/observable.h>
#include "module/common/text_encoding.h"

namespace ra::mod::tool::md5 {

class MD5Result {
public:
	std::uint64_t total_size{};
	std::uint64_t current_size{};
	std::wstring md5;
};

zaf::Observable<MD5Result> CalculateFileMD5(const std::filesystem::path& file_path);

std::wstring CalculateStringMD5(const std::wstring& string, TextEncoding encoding);

}