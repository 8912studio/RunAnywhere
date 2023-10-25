#include "module/chat_gpt/chat_gpt_answer_view.h"
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include "help/help_style_config.h"
#include "module/chat_gpt/progress_indicator.h"
#include "utility/markdown/parse/markdown_parser.h"

using namespace ra::utility::markdown::parse;
using namespace ra::utility::markdown::render;

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTAnswerView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_answer_view.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTAnswerView::SetAnswer(zaf::Observable<std::wstring> observable_answer) {

    auto progress_indicator = zaf::Create<ProgressIndicator>();
    progress_indicator->StartAnimation();
    progress_indicator->SetFixedHeight(90);
    ShowContent(progress_indicator);

    Subscriptions() += observable_answer.Subscribe([this](const std::wstring& answer) {

        auto root_element = MarkdownParser::Instance()->Parse(answer);
        markdown_region_ = MarkdownRegion::Create(*root_element, help::GetHelpStyleConfig());
        ShowContent(markdown_region_);
    },
    [this](const zaf::Error& error) {
        ShowError(error);
    });
}


void ChatGPTAnswerView::ChangeStyle(CommandDisplayStyle style) {

    errorView->ChangeStyle(style);
}


void ChatGPTAnswerView::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    if (event_info.PreviousRect().size.width != this->Width()) {
        ResetContentHeight();
    }
}


void ChatGPTAnswerView::ShowContent(const std::shared_ptr<zaf::Control>& content) {

    contentView->RemoveAllChildren();
    contentView->AddChild(content);
    contentView->SetIsVisible(true);
    errorView->SetIsVisible(false);
    ResetContentHeight();
}


void ChatGPTAnswerView::ShowError(const zaf::Error& error) {

    //Invalid question.
    if (error.Code() == zaf::BasicErrc::InvalidValue) {
        errorView->ShowHintText(L"No response for empty message");
    }
    else {
        errorView->ShowErrorText(L"Unknown error");
    }

    errorView->SetIsVisible(true);
    contentView->SetIsVisible(false);
}


void ChatGPTAnswerView::ResetContentHeight() {

    if (!markdown_region_) {
        return;
    }

    auto preferred_size = markdown_region_->CalculatePreferredSize(this->ContentSize());
    markdown_region_->SetFixedHeight(std::ceil(preferred_size.height));
}

}