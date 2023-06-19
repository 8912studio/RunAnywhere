#include "module/user_defined/preview/user_defined_command_preview_control.h"
#include <zaf/object/type_definition.h>
#include "utility/path_trimming.h"

namespace ra::mod::user_defined {
namespace {

constexpr float LabelHeight = 25;
constexpr float LabelFontSize = 14;

}

ZAF_DEFINE_TYPE(UserDefinedCommandPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(
    L"res:///module/user_defined/preview/user_defined_command_preview_control.xaml")
ZAF_DEFINE_TYPE_END


void UserDefinedCommandPreviewControl::AfterParse() {

    __super::AfterParse();

    auto text_trimming = utility::CreateTextTrimmingForPath();

    commandLabel->SetFixedHeight(LabelHeight);
    commandLabel->SetTextTrimming(text_trimming);
    commandLabel->SetFontSize(LabelFontSize);

    workingDirectoryLabel->SetFixedHeight(LabelHeight);
    workingDirectoryLabel->SetTextTrimming(text_trimming);
    workingDirectoryLabel->SetFontSize(LabelFontSize);
}


void UserDefinedCommandPreviewControl::SetExecutInfo(const ExecuteInfo& execute_info) {

    commandLabel->SetText(execute_info.command_line.command);
    BuildArgumentLabels(execute_info.command_line.arguments);

    if (!execute_info.working_directory.empty()) {
        workingDirectoryLabel->SetIsVisible(true);
        workingDirectoryLabel->SetText(L'(' + execute_info.working_directory + L')');
    }
    else {
        workingDirectoryLabel->SetIsVisible(false);
    }
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