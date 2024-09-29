#include "module/ai/gpt/dialog/list/dialog_item_view.h"
#include <format>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogItemView);

DialogItemView::DialogItemView(std::shared_ptr<DialogItemData> dialog_data) : 
    dialog_data_(std::move(dialog_data)) {

}


void DialogItemView::AfterParse() {

    __super::AfterParse();

    this->SetText(GenerateSubject());
}


std::wstring DialogItemView::GenerateSubject() const {

    if (!dialog_data_->Entity().subject.empty()) {
        return zaf::FromUTF8String(dialog_data_->Entity().subject);
    }

    return std::format(L"New dialog #{}", dialog_data_->TransientID());
}

}