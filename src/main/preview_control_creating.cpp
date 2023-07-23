#include "main/preview_control_creating.h"
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include "utility/path_trimming.h"

namespace ra {
namespace {

std::shared_ptr<mod::CommandPreviewControl> CreateDefaultPreviewControl(
    const std::wstring& preview_text) {

    auto label = zaf::Create<zaf::Label>();
    label->SetTextAlignment(zaf::TextAlignment::Leading);
    label->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    label->SetFontSize(14);
    label->SetTextTrimming(utility::CreateTextTrimmingForPath());
    label->SetText(preview_text);

    auto result = zaf::Create<mod::CommandPreviewControl>();
    result->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    result->SetFixedHeight(40);
    result->AddChild(label);
    return result;
}

}

std::shared_ptr<mod::CommandPreviewControl> CreateCommandPreviewControl(mod::Command& command) {

    auto preview_control = command.GetPreviewControl();
    if (!preview_control) {
        preview_control = CreateDefaultPreviewControl(command.GetPreviewText());
    }

    preview_control->SetMargin(preview_control->GetExpectedMargin());
    return preview_control;
}

}