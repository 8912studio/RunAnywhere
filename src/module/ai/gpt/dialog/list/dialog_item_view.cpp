#include "module/ai/gpt/dialog/list/dialog_item_view.h"
#include <format>
#include <zaf/base/as.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogItemView);

void DialogItemView::AfterParse() {

    __super::AfterParse();

    Label()->SetTextTrimming(zaf::dwrite::TextTrimmingGranularity::Character);
}


void DialogItemView::OnItemDataChanged() {

    Label()->SetText(GenerateSubject());
}


std::wstring DialogItemView::GenerateSubject() const {

    auto dialog = zaf::As<Dialog>(ItemData());
    if (!dialog) {
        return {};
    }

    if (!dialog->Entity().subject.empty()) {
        return zaf::FromUTF8String(dialog->Entity().subject);
    }

    auto transient_id = dialog->ID().TransientID();
    if (transient_id) {
        return std::format(L"New dialog #{}", transient_id->Value());
    }

    return L"New dialog";
}

}