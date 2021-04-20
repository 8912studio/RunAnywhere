#include "module/user_defined/preview/user_defined_command_preview_control.h"
#include <zaf/reflection/reflection_type_definition.h>

namespace {

constexpr float LabelHeight = 25;
constexpr float LabelFontSize = 14;

zaf::TextTrimming CreateTextTrimming() {

    zaf::TextTrimming text_trimming;
    text_trimming.granularity = zaf::TextTrimming::Granularity::Character;
    text_trimming.delimiter = L'\\';
    text_trimming.delimiter_count = 2;
    return text_trimming;
}

}

ZAF_DEFINE_REFLECTION_TYPE(UserDefinedCommandPreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/user_defined/preview/user_defined_command_preview_control.xaml")
ZAF_DEFINE_END


void UserDefinedCommandPreviewControl::AfterParsing() {

    __super::AfterParsing();

    commandLabel->SetFixedHeight(LabelHeight);
    commandLabel->SetTextTrimming(CreateTextTrimming());
    commandLabel->SetFontSize(LabelFontSize);
}


void UserDefinedCommandPreviewControl::SetParseResult(const ParseResult& parse_result) {

    commandLabel->SetText(parse_result.command);
    BuildArgumentLabels(parse_result.arguments);
    ResetFixedHeight();
}


void UserDefinedCommandPreviewControl::BuildArgumentLabels(
    const std::vector<std::wstring>& arguments) {

    argumentContainer->RemoveAllChildren();

    std::wstring ident(4, L' ');
    auto text_trimming = CreateTextTrimming();

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


void UserDefinedCommandPreviewControl::ResetFixedHeight() {

    float height = commandLabel->GetPreferredSize().height;

    for (const auto& each_child : argumentContainer->GetChildren()) {
        height += each_child->GetPreferredSize().height;
    }

    this->SetFixedHeight(height);
}