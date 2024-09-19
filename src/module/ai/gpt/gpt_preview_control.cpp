#include "module/ai/gpt/gpt_preview_control.h"
#include <zaf/graphic/alignment.h>
#include "module/ai/gpt/gpt_question_view.h"

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(GPTPreviewControl);

void GPTPreviewControl::ShowQuestion(const std::wstring& question) {

    auto question_view = zaf::Create<GPTQuestionView>();
    question_view->SetQuestion(question);
    ShowContentView(question_view);
}


void GPTPreviewControl::ShowContentView(const std::shared_ptr<zaf::Control>& view) {

    contentView->RemoveAllChildren();
    contentView->AddChild(view);
}


void GPTPreviewControl::OnStyleChanged() {

    __super::OnStyleChanged();

    if (contentView->ChildCount() == 0) {
        return;
    }

    const auto& child = contentView->Children().front();
    if (auto question_view = zaf::As<GPTQuestionView>(child)) {
        question_view->ChangeStyle(this->Style());
    }
}

}