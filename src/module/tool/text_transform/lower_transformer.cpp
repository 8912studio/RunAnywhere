#include "module/tool/text_transform/lower_transformer.h"
#include <zaf/base/string/case_conversion.h>

namespace ra::module::tool::text_transform {

CommandBrief LowerTransformer::Brief() {
    return CommandBrief{ L"low", L"Transform text to lowercase." };
}


CommandBrief LowerTransformer::GetBrief() {
    return Brief();
}


help::content::Content LowerTransformer::GetHelpContent() {
    return {};
}


void LowerTransformer::Transform(std::wstring& text) {
    zaf::Lowercase(text);
}

}