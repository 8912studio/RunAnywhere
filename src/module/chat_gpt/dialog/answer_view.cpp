#include "module/chat_gpt/dialog/answer_view.h"
#include <zaf/control/text_box.h>
#include <zaf/object/type_definition.h>
#include <curlion.h>
#include "help/help_style_config.h"
#include "module/chat_gpt/comm/error.h"
#include "module/chat_gpt/local_error.h"
#include "module/chat_gpt/progress_indicator.h"
#include "module/common/style_constants.h"
#include "utility/markdown/parse/markdown_parser.h"

using namespace ra::utility::markdown::parse;
using namespace ra::utility::markdown::render;

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(AnswerView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/dialog/answer_view.xaml")
ZAF_DEFINE_TYPE_END;

void AnswerView::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    if (event_info.PreviousRect().size.width != this->Width()) {
        ResetContentHeight();
    }
}


void AnswerView::SetAnswer(zaf::Observable<std::wstring> observable_answer) {

    //Display progress indicator when waiting for the answer.
    auto progress_indicator = zaf::Create<ProgressIndicator>();
    progress_indicator->StartAnimation();
    contentView->SetChildren({ progress_indicator });

    Subscriptions() += observable_answer.Subscribe([this](const std::wstring& answer) {
        ShowAnswer(answer);
    },
    [this](const zaf::Error& error) {
        ShowError(error);
    });
}


void AnswerView::ShowAnswer(const std::wstring& answer) {

    auto root_element = MarkdownParser::Instance()->Parse(answer);

    auto style_config = help::GetHelpStyleConfig();
    style_config.basic_config.font.size = StyleConstants::PreservedBodyFontSize;
    style_config.code_block_config.show_header = true;

    markdown_region_ = MarkdownRegion::Create(*root_element, style_config);
    markdown_region_->SetCanSelect(true);

    contentView->SetChildren({ markdown_region_ });
    ResetContentHeight();
}


void AnswerView::ShowError(const zaf::Error& error) {

    auto error_text = [&error]() -> std::wstring {

        if (error.Code() == LocalErrc::NoAPIKey) {
            return L"No API key";
        }
        if (error.Code() == LocalErrc::ChatOngoing) {
            return L"A chat is in progress, try again later";
        }

        const auto& error_category = error.Code().category();

        if (error_category == comm::CURLErrorCategory() ||
            error_category == comm::HTTPErrorCategory() ||
            error_category == curlion::CurlMultiErrorCategory()) {
            return L"Network error";
        }

        return L"Unknown error";
    }();

    auto error_text_box = zaf::Create<zaf::TextBox>();
    error_text_box->SetAutoHeight(true);
    error_text_box->SetWordWrapping(zaf::WordWrapping::Wrap);
    error_text_box->SetFontSize(StyleConstants::PreservedBodyFontSize);
    error_text_box->SetTextColor(zaf::Color::FromRGB(0xEE4444));
    error_text_box->SetText(error_text);
    contentView->SetChildren({ error_text_box });
}


void AnswerView::ResetContentHeight() {

    if (!markdown_region_) {
        return;
    }

    auto preferred_size = markdown_region_->CalculatePreferredSize(this->ContentSize());
    markdown_region_->SetFixedHeight(std::ceil(preferred_size.height));
}


}