#include "module/chat_gpt/chat_gpt_preview_control.h"
#include <zaf/graphic/alignment.h>
#include <zaf/object/type_definition.h>
#include "module/chat_gpt/chat_gpt_answer_view.h"
#include "module/chat_gpt/chat_gpt_question_view.h"

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTPreviewControl::ShowQuestion(const std::wstring& question) {

    auto question_view = zaf::Create<ChatGPTQuestionView>();
    question_view->SetQuestion(question);
    ShowContentView(question_view);
}


void ChatGPTPreviewControl::ShowAnswer(zaf::Observable<std::wstring> observable_answer) {

    auto answer_view = zaf::Create<ChatGPTAnswerView>();
    answer_view->SetAnswer(observable_answer);
    ShowContentView(answer_view);

    Subscriptions() += observable_answer.Subscribe([this](const std::wstring&) {
        RaiseContentChangedEvent();
    });
}


void ChatGPTPreviewControl::ShowContentView(const std::shared_ptr<zaf::Control>& view) {

    contentView->RemoveAllChildren();
    contentView->AddChild(view);
}


zaf::Frame ChatGPTPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 5;
    return result;
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
    else if (auto answer_view = zaf::As<ChatGPTAnswerView>(child)) {
        answer_view->ChangeStyle(this->Style());
    }
}

}