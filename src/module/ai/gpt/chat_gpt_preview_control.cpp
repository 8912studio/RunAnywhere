#include "module/ai/gpt/chat_gpt_preview_control.h"
#include <zaf/graphic/alignment.h>
#include "module/ai/gpt/chat_gpt_question_view.h"

namespace ra::mod::chat_gpt {

ZAF_OBJECT_IMPL(ChatGPTPreviewControl);

void ChatGPTPreviewControl::ShowQuestion(const std::wstring& question) {

    auto question_view = zaf::Create<ChatGPTQuestionView>();
    question_view->SetQuestion(question);
    ShowContentView(question_view);
}


void ChatGPTPreviewControl::ShowContentView(const std::shared_ptr<zaf::Control>& view) {

    contentView->RemoveAllChildren();
    contentView->AddChild(view);
}


void ChatGPTPreviewControl::OnStyleChanged() {

    __super::OnStyleChanged();

    if (contentView->ChildCount() == 0) {
        return;
    }

    const auto& child = contentView->Children().front();
    if (auto question_view = zaf::As<ChatGPTQuestionView>(child)) {
        question_view->ChangeStyle(this->Style());
    }
}

}