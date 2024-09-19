#include "module/ai/gpt/gpt_question_view.h"
#include "application_delegate.h"
#include "module/common/error_messages.h"
#include "option/option_storage.h"
#include "option/option_window.h"

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(GPTQuestionView);

void GPTQuestionView::AfterParse() {

    __super::AfterParse();

    TextDisplayMode display_mode;
    display_mode.word_wrapping = zaf::dwrite::WordWrapping::Wrap;
    questionControl->SetDisplayMode(display_mode);

    Subscriptions() += openOptionButton->ClickEvent().Subscribe(std::bind([]() {
        ApplicationDelegate::GetFromApplication()->HideMainWindow();
        option::OptionWindow::ShowInstance();
    }));
}


void GPTQuestionView::SetQuestion(const std::wstring& question) {

    if (option::OptionStorage::Instance().OpenAIAPIKey().empty()) {
        ShowNoAPIKeyError();
    }
    else {
        ShowQuestion(question);
    }
}


void GPTQuestionView::ShowNoAPIKeyError() {

    messageLabel->SetIsVisible(false);

    errorView->ShowErrorText(L"API key is required");
    errorContainer->SetIsVisible(true);
    openOptionButton->SetIsVisible(true);
}


void GPTQuestionView::ShowQuestion(const std::wstring& question) {

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


void GPTQuestionView::ChangeStyle(CommandDisplayStyle style) {

    questionControl->ChangeStyle(style);
    errorView->ChangeStyle(style);
}

}