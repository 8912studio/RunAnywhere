#include "module/tool/text_transform/upper_transformer.h"
#include <zaf/base/string/case_conversion.h>
#include "help/built_in_help_content_manager.h"

namespace ra::mod::tool::text_transform {

CommandBrief UpperTransformer::Brief() {
    return CommandBrief{ L"up", L"Transform text to uppercase" };
}


CommandBrief UpperTransformer::GetBrief() {
    return Brief();
}


std::shared_ptr<help::markdown::element::Element> UpperTransformer::GetHelpContent() {
	return help::BuiltInHelpContentManager::Instance().GetDetail(Brief().Command());
}


void UpperTransformer::Transform(std::wstring & text) {
    zaf::Uppercase(text);
}

}