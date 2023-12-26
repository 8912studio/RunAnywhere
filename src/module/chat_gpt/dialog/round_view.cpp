#include "module/chat_gpt/dialog/round_view.h"
#include <zaf/object/type_definition.h>

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(RoundView);
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/dialog/round_view.xaml")
ZAF_DEFINE_TYPE_END;

RoundView::RoundView(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

}


void RoundView::AfterParse() {

    __super::AfterParse();
}


void RoundView::Start(std::wstring question) {

    questionContent->SetText(question);

    auto observable = dialog_->Chat(std::move(question)).Map<std::wstring>(
        [](const comm::ChatCompletion& chat_completion) {
        return chat_completion.Message().Content();
    });

    answerView->SetAnswer(observable);
}

}