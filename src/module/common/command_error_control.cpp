#include "module/common/command_error_control.h"
#include <zaf/creation.h>
#include <zaf/control/layout/linear_layouter.h>

namespace ra::mod {

std::shared_ptr<CommandErrorControl> CommandErrorControl::InvalidArgument() {
    auto result = zaf::Create<CommandErrorControl>();
    result->SetErrorText(L"Invalid argument");
    return result;
}


void CommandErrorControl::AfterParse() {

    __super::AfterParse();

    error_view_ = zaf::Create<ErrorView>();
    AddChild(error_view_);

    SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    SetFixedHeight(90);
}


void CommandErrorControl::SetErrorText(const std::wstring& text) {
    error_view_->SetErrorText(text);
}

}