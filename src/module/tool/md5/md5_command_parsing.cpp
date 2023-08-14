#include "module/tool/md5/md5_command_parsing.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::mod::tool::md5 {
namespace {

void ParseNormalText(std::wstring_view text, MD5CommandParseResult& result) {

	auto active_path_option = active_path::TryToParseActivePathArgument(text);
	if (active_path_option) {
		result.active_path_option = active_path_option;
	}
	else if (text.front() == L'/') {

		auto switch_value = text.substr(1);
		if (switch_value == L"u8") {
			result.encoding = TextEncoding::UTF8;
		}
		else if (switch_value == L"u16") {
			result.encoding = TextEncoding::UTF16;
		}
		else if (switch_value == L"f") {
			result.treat_string_as_file = true;
		}
		else if (switch_value == L"c") {
			result.use_uppercase = true;
		}
	}
	else {
		result.string = text;
	}
}

}

MD5CommandParseResult ParseMD5Command(const utility::CommandLine& command_line) {

	MD5CommandParseResult result;

	for (const auto& each_argument : command_line.Arguments()) {

		if (each_argument.Type() == utility::CommandLinePieceType::NormalText) {
			ParseNormalText(each_argument.Content(), result);
		}
		else if (each_argument.Type() == utility::CommandLinePieceType::TextBlock) {
			result.string = each_argument.Content();
		}
	}

	return result;
}

}