#include "module/user_defined/preview/user_defined_command_preview_control.h"
#include <zaf/reflection/reflection_type_definition.h>
#include "utility/path_trimming.h"

namespace ra::module::user_defined {
namespace {

constexpr float LabelHeight = 25;
constexpr float LabelFontSize = 14;

}

ZAF_DEFINE_REFLECTION_TYPE(UserDefinedCommandPreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/user_defined/preview/user_defined_command_preview_control.xaml")
ZAF_DEFINE_END


void UserDefinedCommandPreviewControl::AfterParse() {

    __super::AfterParse();

    commandLabel->SetFixedHeight(LabelHeight);
    commandLabel->SetTextTrimming(utility::CreateTextTrimmingForPath());
    commandLabel->SetFontSize(LabelFontSize);
}


void UserDefinedCommandPreviewControl::SetParseResult(const ParseResult& parse_result) {

    commandLabel->SetText(parse_result.command);
    BuildArgumentLabels(parse_result.arguments);
}


void UserDefinedCommandPreviewControl::BuildArgumentLabels(
    const std::vector<std::wstring>& arguments) {

    argumentContainer->RemoveAllChildren();

    std::wstring ident(4, L' ');
    auto text_trimming = utility::CreateTextTrimmingForPath();

    for (const auto& each_argument : arguments) {

        auto label = zaf::Create<zaf::Label>();
        label->SetFixedHeight(LabelHeight);
        label->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
        label->SetTextTrimming(text_trimming);
        label->SetFontSize(LabelFontSize);
        label->SetText(ident + each_argument);
        
        argumentContainer->AddChild(label);
    }
}

}