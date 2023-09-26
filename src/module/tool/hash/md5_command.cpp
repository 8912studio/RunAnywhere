#include "module/tool/hash/md5_command.h"
#include <zaf/crypto/md5.h>
#include "help/command_help_content_factory.h"

namespace ra::mod::tool::hash {

CommandBrief MD5Command::Brief() {
	return CommandBrief{
		L"md5",
		L"Calculate MD5 hash"
	};
}


CommandBrief MD5Command::GetBrief() {
	return Brief();
}


std::shared_ptr<help::markdown::element::Element> MD5Command::GetHelpContent() {

	help::CommandHelpContentFactory factory(Brief().Command());
	return factory.LoadHelpContent();
}


HashAlgorithmCreator MD5Command::GetHashAlgorithmCreator() {
	return []() {
		return zaf::crypto::MD5{};
	};
}

}