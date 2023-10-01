#include "module/tool/hash/md5_command.h"
#include <zaf/crypto/md5.h>
#include "help/built_in_help_content_manager.h"

namespace ra::mod::tool::hash {

std::wstring MD5Command::Keyword() {
	return L"md5";
}


std::wstring MD5Command::GetKeyword() {
	return Keyword();
}


std::shared_ptr<help::markdown::element::Element> MD5Command::GetHelpContent() {
	return help::BuiltInHelpContentManager::Instance().GetDetail(Keyword());
}


HashAlgorithmCreator MD5Command::GetHashAlgorithmCreator() {
	return []() {
		return zaf::crypto::MD5{};
	};
}

}