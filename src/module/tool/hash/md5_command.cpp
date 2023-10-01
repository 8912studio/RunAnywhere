#include "module/tool/hash/md5_command.h"
#include <zaf/crypto/md5.h>
#include "help/built_in_help_content_manager.h"

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
	return help::BuiltInHelpContentManager::Instance().GetDetail(Brief().Command());
}


HashAlgorithmCreator MD5Command::GetHashAlgorithmCreator() {
	return []() {
		return zaf::crypto::MD5{};
	};
}

}