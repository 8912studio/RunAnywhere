#include "module/crypto/md5/md5_command_parsing.h"
#include "utility/command_line_arguments.h"

namespace ra::module::crypto {

std::optional<MD5CommandParseResult> ParseMD5Command(const std::wstring& command) {

	auto arguments = utility::CommandLineArguments::Parse(command);
	if (arguments.GetCount() <= 0) {
		return std::nullopt;
	}

	if (arguments[0] != L"md5") {
		return std::nullopt;
	}

	MD5CommandParseResult result;

	for (int index = 1; index < arguments.GetCount(); ++index) {

		auto argument = arguments[index];
		if (argument.empty()) {
			continue;
		}

		if (argument.front() == L'/') {
			
			auto switch_value = argument.substr(1);
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
			result.string = argument;
		}
	}

	return result;
}

}