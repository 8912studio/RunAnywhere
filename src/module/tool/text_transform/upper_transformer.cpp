#include "module/tool/text_transform/upper_transformer.h"
#include <zaf/base/string/case_conversion.h>

namespace ra::module::tool::text_transform {

CommandBrief UpperTransformer::Brief() {
    return CommandBrief{ L"up", L"Transform text to uppercase." };
}


CommandBrief UpperTransformer::GetBrief() {
    return Brief();
}


help::content::Content UpperTransformer::GetHelpContent() {
    return {};
}


void UpperTransformer::Transform(std::wstring & text) {
    zaf::Uppercase(text);
}

}