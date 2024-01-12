#include "module/chat_gpt/dialog/answer_view.h"
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/control/text_box.h>
#include <zaf/object/type_definition.h>
#include <curlion.h>
#include "help/help_style_config.h"
#include "module/chat_gpt/comm/error.h"
#include "module/chat_gpt/local_error.h"
#include "module/chat_gpt/progress_indicator.h"
#include "module/common/style_constants.h"
#include "utility/markdown/parse/markdown_parser.h"
#include "utility/markdown/render/styled_text_box.h"

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

    contentView->SetChildren({ markdown_region_ });
    ResetContentHeight();
}


void AnswerView::ShowError(const zaf::Error& error) {

    auto container = zaf::Create<zaf::VerticalBox>();
    container->SetAutoHeight(true);
    container->AddChildren({
        CreateMajorErrorControl(error),
        CreateDetailErrorControl(error),
    });
    contentView->SetChildren({ container });
}


std::shared_ptr<zaf::Control> AnswerView::CreateMajorErrorControl(const zaf::Error& error) {

    StyledText styled_text;
    styled_text.Append([&error]() -> std::wstring {

        if (error.Code() == LocalErrc::NoAPIKey) {
            return L"No API key";
        }
        if (error.Code() == LocalErrc::ChatOngoing) {
            return L"A chat is in progress, try again later";
        }

        const auto& error_category = error.Code().category();

        if (error_category == comm::CURLErrorCategory() ||
            error_category == curlion::CurlMultiErrorCategory()) {
            return L"Network error";
        }

        if (error_category == comm::HTTPErrorCategory()) {
            return L"Server error";
        }

        return L"Unknown error";
    }());

    TextStyle text_style;
    text_style.font.size = StyleConstants::PreservedBodyFontSize;
    text_style.text_color = zaf::Color::FromRGB(0xEE4444);
    styled_text.AddStyleToPendingText(text_style);

    auto result = zaf::Create<StyledTextBox>();
    result->SetAutoHeight(true);
    result->SetWordWrapping(zaf::WordWrapping::Wrap);
    result->SetStyledText(styled_text);
    return result;
}


std::shared_ptr<zaf::Control> AnswerView::CreateDetailErrorControl(const zaf::Error& error) {

    StyledText styled_text;
    styled_text.Append([&error]() -> std::wstring {

        auto result = std::format(
            "{}:{} {}",
            error.Code().category().name(),
            error.Code().value(),
            error.Message());

        return zaf::FromUTF8String(result);
    }());

    TextStyle detail_text_style;
    detail_text_style.font.size = StyleConstants::PreservedBodyFontSize;
    detail_text_style.text_color = zaf::Color::Gray();
    styled_text.AddStyleToPendingText(detail_text_style);

    auto result = zaf::Create<StyledTextBox>();
    result->SetMargin(zaf::Frame{ 0, 4, 0, 0 });
    result->SetWordWrapping(zaf::WordWrapping::Wrap);
    result->SetAutoHeight(true);
    result->SetIsEnabled(true);
    result->SetStyledText(styled_text);
    return result;
}


void AnswerView::ResetContentHeight() {

    if (!markdown_region_) {
        return;
    }

    auto preferred_size = markdown_region_->CalculatePreferredSize(this->ContentSize());
    markdown_region_->SetFixedHeight(std::ceil(preferred_size.height));
}


}