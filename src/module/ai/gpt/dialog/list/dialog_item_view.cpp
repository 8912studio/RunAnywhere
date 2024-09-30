#include "module/ai/gpt/dialog/list/dialog_item_view.h"
#include <format>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogItemView);

DialogItemView::DialogItemView(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

}


void DialogItemView::AfterParse() {

    __super::AfterParse();

    this->SetText(GenerateSubject());
}


std::wstring DialogItemView::GenerateSubject() const {

    if (!dialog_->Entity().subject.empty()) {
        return zaf::FromUTF8String(dialog_->Entity().subject);
    }

    auto transient_id = dialog_->ID().TransientID();
    if (transient_id) {
        return std::format(L"New dialog #{}", transient_id->Value());
    }

    return L"New dialog";
}

}