#include "module/crypto/md5/md5_command_parsing.h"

namespace ra::module::crypto {

std::optional<MD5CommandParseResult> ParseMD5Command(const utility::CommandLine& command_line) {

	if (command_line.Command() != L"md5") {
		return std::nullopt;
	}

	MD5CommandParseResult result;

	for (const auto& each_argument : command_line.Arguments()) {

		if (each_argument.front() == L'/') {
			
			auto switch_value = each_argument.substr(1);
			if (switch_value == L"u8") {
				result.encoding = MD5Encoding::UTF8;
			}
			else if (switch_value == L"u16") {
				result.encoding = MD5Encoding::UTF16;
			}
			else if (switch_value == L"c") {
				result.use_uppercase = true;
			}
		}
		else {
			result.string = each_argument;
		}
	}

	return result;
}

}