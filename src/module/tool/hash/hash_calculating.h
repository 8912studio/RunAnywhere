#pragma once

#include <filesystem>
#include <string>
#include <zaf/rx/observable.h>
#include "module/common/text_encoding.h"
#include "module/tool/hash/hash_algorithm_info.h"

namespace ra::mod::tool::hash {

class HashResult {
public:
	std::uint64_t total_size{};
	std::uint64_t current_size{};
	std::wstring result;
};

zaf::Observable<HashResult> CalculateFileHash(
	const std::filesystem::path& file_path,
	HashAlgorithmCreator hash_algorithm_creator);

std::wstring CalculateStringHash(
	const std::wstring& string, 
	TextEncoding encoding,
	const HashAlgorithmCreator& hash_algorithm_creator);

}