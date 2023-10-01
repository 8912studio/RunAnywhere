#include "module/tool/text_transform/lower_transformer.h"
#include <zaf/base/string/case_conversion.h>
#include "help/built_in_help_content_manager.h"

namespace ra::mod::tool::text_transform {

std::wstring LowerTransformer::Keyword() {
    return L"low";
}


std::wstring LowerTransformer::GetKeyword() {
    return Keyword();
}


std::shared_ptr<help::markdown::element::Element> LowerTransformer::GetHelpContent() {
	return help::BuiltInHelpContentManager::Instance().GetDetail(Keyword());
}


void LowerTransformer::Transform(std::wstring& text) {
    zaf::Lowercase(text);
}

}