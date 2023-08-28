#include "module/tool/hash/hash_command_parsing.h"

namespace ra::mod::tool::hash {
namespace {

bool ParseNormalText(std::wstring_view text, HashCommandParseResult& result) {
	if (text == L"/c") {
		result.use_uppercase = true;
		return true;
	}
	return false;
}

}

HashCommandParseResult ParseHashCommand(const utility::CommandLine& command_line) {

	HashCommandParseResult result;

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