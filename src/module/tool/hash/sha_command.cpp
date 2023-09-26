#include "module/tool/hash/sha_command.h"
#include <zaf/crypto/sha.h>
#include "module/tool/hash/sha_algorithm_type_parsing.h"

namespace ra::mod::tool::hash {

CommandBrief SHACommand::Brief() {
	return CommandBrief{
		L"sha",
		L"Calculate SHA hash"
	};
}


CommandBrief SHACommand::GetBrief() {
	return Brief();
}


std::shared_ptr<help::markdown::element::Element> SHACommand::GetHelpContent() {
	return nullptr;
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