#include "module/chat_gpt/chat_gpt_question_view.h"
#include <zaf/object/type_definition.h>
#include "application_delegate.h"
#include "module/common/error_messages.h"
#include "option/option_storage.h"
#include "option/option_window.h"

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTQuestionView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_question_view.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTQuestionView::AfterParse() {

    __super::AfterParse();

    TextDisplayMode display_mode;
    display_mode.word_wrapping = zaf::WordWrapping::Wrap;
    questionControl->SetDisplayMode(display_mode);

    Subscriptions() += openOptionButton->ClickEvent().Subscribe(std::bind([]() {
        ApplicationDelegate::GetFromApplication()->HideMainWindow();
        option::OptionWindow::ShowInstance();
    }));
}


void ChatGPTQuestionView::SetQuestion(const std::wstring& question) {

    if (option::OptionStorage::Instance().OpenAIAPIKey().empty()) {
        ShowNoAPIKeyError();
    }
    else {
        ShowQuestion(question);
    }
}


void ChatGPTQuestionView::ShowNoAPIKeyError() {

    messageLabel->SetIsVisible(false);

    errorView->ShowErrorText(L"API key is required");
    errorContainer->SetIsVisible(true);
    openOptionButton->SetIsVisible(true);
}


void ChatGPTQuestionView::ShowQuestion(const std::wstring& question) {

    messageLabel->SetIsVisible(true);

    if (!question.empty()) {
        questionControl->SetText(question);
        questionControl->SetIsVisible(true);
    }
    else {
        errorView->ShowHintText(ErrorMessages::NoContentToDisplay);
        errorContainer->SetIsVisible(true);
    }
}


void ChatGPTQuestionView::ChangeStyle(CommandDisplayStyle style) {

    questionControl->ChangeStyle(style);
    errorView->ChangeStyle(style);
}

}