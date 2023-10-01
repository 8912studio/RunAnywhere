#include "module/tool/text_transform/lower_transformer.h"
#include <zaf/base/string/case_conversion.h>
#include "help/built_in_help_content_manager.h"

namespace ra::mod::tool::text_transform {

CommandBrief LowerTransformer::Brief() {
    return CommandBrief{ L"low", L"Transform text to lowercase" };
}


CommandBrief LowerTransformer::GetBrief() {
    return Brief();
}


std::shared_ptr<help::markdown::element::Element> LowerTransformer::GetHelpContent() {
	return help::BuiltInHelpContentManager::Instance().GetDetail(Brief().Command());
}


void LowerTransformer::Transform(std::wstring& text) {
    zaf::Lowercase(text);
}

}