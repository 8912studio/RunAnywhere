#include "preview_view.h"
#include <zaf/base/error/check.h>
#include <zaf/reflection/reflection_type_definition.h>

ZAF_DEFINE_REFLECTION_TYPE(PreviewView)
    ZAF_DEFINE_RESOURCE_URI(L"res:///preview_view.xaml");
ZAF_DEFINE_END


void PreviewView::SetPreviewControl(const std::shared_ptr<CommandPreviewControl>& control) {

    ZAF_EXPECT(control);

    ClearPreviewControl();

    preview_control_ = control;
    this->AddChild(preview_control_);
}


void PreviewView::ClearPreviewControl() {

    if (preview_control_) {

        this->RemoveChild(preview_control_);
        preview_control_ = nullptr;
    }
}


float PreviewView::GetExpectedHeight() {

    if (!preview_control_) {
        return 0;
    }

    float result = preview_control_->GetPreferredSize().height;

    const auto& preview_control_margin = preview_control_->GetMargin();
    result += preview_control_margin.top + preview_control_margin.bottom;

    const auto& margin = this->GetMargin();
    result += margin.top + margin.bottom;

    return result;
}