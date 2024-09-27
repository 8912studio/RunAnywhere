#include "module/ai/gpt/dialog/dialog_item_view.h"

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogItemView);

DialogItemView::DialogItemView(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

}


void DialogItemView::AfterParse() {

    __super::AfterParse();

    this->SetText(dialog_->Subject());
}

}