#include "module/user_defined/preview/user_defined_command_preview_control.h"
#include <zaf/base/as.h>
#include "module/common/style_constants.h"
#include "module/user_defined/bundle_definition.h"
#include "utility/path_trimming.h"

namespace ra::mod::user_defined {
namespace {

constexpr float MinCommandContentHeight = 30;
constexpr float MaxCommandContentHeight = 240;

template<bool IsPreserved>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
    static constexpr float LabelHeight = 24;
    static constexpr float LabelFontSize = 16;

    static zaf::Frame Margin() {
        return zaf::Frame{ 0, 8, 0, 10 };
    }
};

template<>
struct StyleMetrics<true> {

    static constexpr float LabelHeight = 22;
    static constexpr float LabelFontSize = StyleConstants::PreservedBodyFontSize;

    static zaf::Frame Margin() {
        return {};
    }
};

}

ZAF_OBJECT_IMPL(UserDefinedCommandPreviewControl);

void UserDefinedCommandPreviewControl::AfterParse() {

    __super::AfterParse();

    auto text_trimming = utility::CreateTextTrimmingForPath();
    commandLabel->SetTextTrimming(text_trimming);
    workingDirectoryLabel->SetTextTrimming(text_trimming);
}


void UserDefinedCommandPreviewControl::OnStyleChanged() {

    auto set_style = [this](auto metrics) {

        commandLabel->SetFixedHeight(metrics.LabelHeight);
        commandLabel->SetFontSize(metrics.LabelFontSize);

        workingDirectoryLabel->SetFixedHeight(metrics.LabelHeight);
        workingDirectoryLabel->SetFontSize(metrics.LabelFontSize);

        for (const auto& each_child : argumentContainer->Children()) {
            auto label = zaf::As<zaf::Label>(each_child);
            if (label) {
                label->SetFixedHeight(metrics.LabelHeight);
                label->SetFontSize(metrics.LabelFontSize);
            }
        }

        scrollContainer->SetMargin(metrics.Margin());
    };

    if (Style() == CommandDisplayStyle::Preserved) {
        set_style(StyleMetrics<true>{});
    }
    else {
        set_style(StyleMetrics<false>{});
    }

    ResetHeight();
}


void UserDefinedCommandPreviewControl::SetCommandEntry(const std::shared_ptr<Entry>& entry) {

    ZAF_EXPECT(entry);
    entry_ = entry;
    entryInfoLabel->SetText(entry_->BundleMeta()->BundleID() + BundleFileExtension);
}


void UserDefinedCommandPreviewControl::SetExecutInfo(ExecuteInfo execute_info) {

    execute_info_ = std::move(execute_info);

    commandLabel->SetText(execute_info_.command_line.command);
    BuildArgumentLabels(execute_info_.command_line.arguments);

    if (!execute_info_.working_directory.empty()) {
        workingDirectoryLabel->SetIsVisible(true);
        workingDirectoryLabel->SetText(L'(' + execute_info_.working_directory + L')');
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
        label->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
        label->SetTextTrimming(text_trimming);
        label->SetText(ident + each_argument);
        argumentContainer->AddChild(label);
    }
}


void UserDefinedCommandPreviewControl::ResetHeight() {

    float container_height = commandContainer->Height();
    float content_height = container_height;

    if (Style() != CommandDisplayStyle::Preserved) {

        container_height = std::max(container_height, MinCommandContentHeight);
        content_height = container_height;

        container_height = std::min(container_height, MaxCommandContentHeight);
    }

    scrollContent->SetFixedHeight(content_height);
    scrollContainer->SetFixedHeight(container_height);
}


zaf::Frame UserDefinedCommandPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 0;
    return result;
}

}