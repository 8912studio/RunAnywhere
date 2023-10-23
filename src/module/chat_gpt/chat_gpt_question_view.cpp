#include "module/chat_gpt/chat_gpt_question_view.h"
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTQuestionView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_question_view.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTQuestionView::AfterParse() {

    __super::AfterParse();

    TextDisplayMode display_mode;
    display_mode.word_wrapping = zaf::WordWrapping::Wrap;
    questionControl->SetDisplayMode(display_mode);
}


void ChatGPTQuestionView::SetQuestion(const std::wstring& question) {

    if (!question.empty()) {
        questionControl->SetText(question);
        questionControl->SetIsVisible(true);
    }
    else {
        errorView->ShowHintText(ErrorMessages::NoContentToDisplay);
        errorView->SetIsVisible(true);
    }
}


void ChatGPTQuestionView::ChangeStyle(CommandDisplayStyle style) {

    questionControl->ChangeStyle(style);
    errorView->ChangeStyle(style);
}

}