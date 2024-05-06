#include "preview_view.h"
#include <zaf/base/error/contract_error.h>
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(PreviewView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///preview_view.xaml");
ZAF_DEFINE_TYPE_END


void PreviewView::SetPreviewControl(const std::shared_ptr<mod::CommandPreviewControl>& control) {

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

    float result = preview_control_->MinHeight();

    const auto& preview_control_margin = preview_control_->Margin();
    result += preview_control_margin.top + preview_control_margin.bottom;

    const auto& margin = this->Margin();
    result += margin.top + margin.bottom;

    return result;
}

}