#include "module/crypto/crypto_module.h"
#include "module/crypto/md5/md5_command.h"
#include "module/crypto/md5/md5_command_parsing.h"

namespace ra::module::crypto {

std::vector<CommandBrief> CryptoModule::QuerySuggestedCommands(const std::wstring& command_text) {

	std::wstring md5_command{ MD5CommandLiteral };
	if (md5_command.find(command_text) == 0) {

		return {
			CommandBrief{ md5_command, L"Calculate MD5 hash" },
		};
	}

	return {};
}


std::shared_ptr<Command> CryptoModule::Interpret(const utility::CommandLine& command_line) {

	auto md5_parse_result = ParseMD5Command(command_line);
	if (md5_parse_result) {
		return std::make_shared<MD5Command>(*md5_parse_result);
	}

	return nullptr;
}

}