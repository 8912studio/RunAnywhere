#include "module/tool/md5/md5_command_parsing.h"

namespace ra::mod::tool::md5 {
namespace {

bool ParseNormalText(std::wstring_view text, MD5CommandParseResult& result) {
	if (text == L"/c") {
		result.use_uppercase = true;
		return true;
	}
	return false;
}

}

MD5CommandParseResult ParseMD5Command(const utility::CommandLine& command_line) {

	MD5CommandParseResult result;

	for (const auto& each_argument : command_line.Arguments()) {

		if (each_argument.Type() == utility::CommandLinePieceType::NormalText) {
			if (ParseNormalText(each_argument.Content(), result)) {
				continue;
			}
		}

		ParseGeneralOption(each_argument, result.general_option);
	}

	return result;
}

}