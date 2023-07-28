#include "main/preview_control_creating.h"
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include "module/common/default_preview_control.h"
#include "utility/path_trimming.h"

namespace ra {
namespace {

std::shared_ptr<mod::CommandPreviewControl> CreateDefaultPreviewControl(
    std::wstring preview_text) {

    auto result = zaf::Create<mod::DefaultPreviewControl>();
    result->SetText(preview_text);
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