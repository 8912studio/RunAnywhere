#include "main/preview_control_creating.h"
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include "utility/path_trimming.h"
#include "module/common/text_content_control.h"

namespace ra {
namespace {

std::shared_ptr<mod::CommandPreviewControl> CreateDefaultPreviewControl(
    std::wstring preview_text) {

    auto text_content_control = zaf::Create<mod::TextContentControl>();
    text_content_control->SetText(std::move(preview_text));

    auto result = zaf::Create<mod::CommandPreviewControl>();
    result->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    result->SetAutoHeight(true);
    result->AddChild(text_content_control);
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