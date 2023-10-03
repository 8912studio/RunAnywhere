#include "module/tool/hash/sha_command.h"
#include <zaf/crypto/sha.h>
#include "help/built_in_help_content_manager.h"
#include "module/tool/hash/sha_algorithm_type_parsing.h"

namespace ra::mod::tool::hash {

std::wstring SHACommand::Keyword() {
	return L"sha";
}


std::wstring SHACommand::GetKeyword() {
	return Keyword();
}


help::HelpContent SHACommand::GetHelpContent() {
	return {
		Keyword(),
		help::BuiltInHelpContentManager::Instance().GetDetail(Keyword()) 
	};
}


HashCommandParseResult SHACommand::ParseCommand(const utility::CommandLine& command_line) {

	sha_algorithm_type = ParseSHAAlgorithmType(command_line);
	return __super::ParseCommand(command_line);
}


HashAlgorithmCreator SHACommand::GetHashAlgorithmCreator() {

	switch (sha_algorithm_type.value_or(SHAAlgorithmType::SHA256)) {
	case SHAAlgorithmType::SHA1:
		return []() { return zaf::crypto::SHA1{}; };
	case SHAAlgorithmType::SHA256:
		return []() { return zaf::crypto::SHA256{}; };
	case SHAAlgorithmType::SHA384:
		return []() { return zaf::crypto::SHA384{}; };
	case SHAAlgorithmType::SHA512:
		return []() { return zaf::crypto::SHA512{}; };
	default:
		ZAF_NOT_REACHED();
	}
}

}