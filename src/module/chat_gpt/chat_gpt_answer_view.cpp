#include "module/chat_gpt/chat_gpt_answer_view.h"
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include <curlion.h>
#include "help/help_style_config.h"
#include "module/chat_gpt/comm/error.h"
#include "module/chat_gpt/local_error.h"
#include "utility/markdown/parse/markdown_parser.h"

using namespace ra::utility::markdown::parse;
using namespace ra::utility::markdown::render;

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTAnswerView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_answer_view.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTAnswerView::SetAnswer(zaf::Observable<AnswerResult> observable_answer) {

    //Display progress indicator when waiting for the answer.
    progressIndicator->StartAnimation();
    progressIndicator->SetIsVisible(true);

    Subscriptions() += observable_answer.Subscribe([this](const AnswerResult& result) {

        if (auto answer = result.Answer()) {
            ShowAnswer(*answer);
        }
        else if (auto error = result.Error()) {
            ShowError(*error);
        }

        progressIndicator->StopAnimation();
        progressIndicator->SetIsVisible(false);
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


void ChatGPTAnswerView::ShowAnswer(const std::wstring& answer) {

    auto root_element = MarkdownParser::Instance()->Parse(answer);
    markdown_region_ = MarkdownRegion::Create(*root_element, help::GetHelpStyleConfig());
    markdown_region_->SetCanSelect(true);
    ShowContent(markdown_region_);
}


void ChatGPTAnswerView::ShowContent(const std::shared_ptr<zaf::Control>& content) {

    contentView->RemoveAllChildren();
    contentView->AddChild(content);
    contentView->SetIsVisible(true);
    errorView->SetIsVisible(false);
    ResetContentHeight();
}


void ChatGPTAnswerView::ShowError(const zaf::Error& error) {

    auto error_text = [&error]() -> std::wstring {
        if (error.Code() == LocalErrc::EmptyQuestion) {
            return L"No response for empty message";
        }
        if (error.Code() == LocalErrc::NoAPIKey) {
            return L"No API key";
        }

        const auto& error_category = error.Code().category();

        if (error_category == comm::CURLErrorCategory() ||
            error_category == comm::HTTPErrorCategory() ||
            error_category == curlion::CurlMultiErrorCategory()) {
            return L"Network error";
        }

        return L"Unknown error";
    }();
    
    errorView->ShowErrorText(error_text);
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