#include "module/common/default_preview_control.h"
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>

namespace ra::mod {

void DefaultPreviewControl::Initialize() {

    __super::Initialize();

    this->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    this->SetAutoHeight(true);

    text_control_ = zaf::Create<TextContentControl>();
    TextDisplayMode display_mode;
    display_mode.word_wrapping = zaf::WordWrapping::WholeWord;
    text_control_->SetDisplayMode(display_mode);
    this->AddChild(text_control_);
}


void DefaultPreviewControl::OnStyleChanged() {

    text_control_->ChangeStyle(Style());
}


void DefaultPreviewControl::SetText(const std::wstring& text) {

    text_control_->SetText(text);
}


zaf::Frame DefaultPreviewControl::GetExpectedMargin() {
    return zaf::Frame{ 15, 0, 15, 0 };
}

}