#include "module/chat_gpt/dialog/answer_view.h"
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/control/text_box.h>
#include <zaf/control/textual/styled_text.h>
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
    [this](const std::exception_ptr& error) {
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


void AnswerView::ShowError(const std::exception_ptr& error) {

    auto container = zaf::Create<zaf::VerticalBox>();
    container->SetAutoHeight(true);
    container->AddChildren({
        CreateMajorErrorControl(error),
        CreateDetailErrorControl(error),
    });
    contentView->SetChildren({ container });
}


std::shared_ptr<zaf::Control> AnswerView::CreateMajorErrorControl(
    const std::exception_ptr& error) {

    zaf::textual::StyledText styled_text;
    zaf::Font font;
    font.size = StyleConstants::PreservedBodyFontSize;
    styled_text.SetDefaultFont(font);
    styled_text.SetDefaultTextColor(zaf::Color::FromRGB(0xEE4444));

    styled_text.SetText([&error]() -> std::wstring {
        try {
            std::rethrow_exception(error);
        }
        catch (const LocalError& local_error) {
            if (local_error.Code() == LocalErrorCode::NoAPIKey) {
                return L"No API key";
            }
            else if (local_error.Code() == LocalErrorCode::ChatOngoing) {
                return L"A chat is in progress, try again later";
            }
        }
        catch (const comm::CURLError&) {
            return L"Network error";
        }
        catch (const comm::CURLMultiSocketError&) {
            return L"Network error";
        }
        catch (const comm::HTTPError&) {
            return L"Server error";
        }
        catch (...) {

        }
        return L"Unknown error";
    }());

    auto result = zaf::Create<zaf::TextBox>();
    result->SetIsEnabled(false);
    result->SetAutoHeight(true);
    result->SetWordWrapping(zaf::WordWrapping::Wrap);
    result->SetStyledText(std::move(styled_text));
    return result;
}


std::shared_ptr<zaf::Control> AnswerView::CreateDetailErrorControl(
    const std::exception_ptr& error) {

    zaf::textual::StyledText styled_text;
    zaf::Font font;
    font.size = StyleConstants::PreservedBodyFontSize;
    styled_text.SetDefaultFont(font);
    styled_text.SetDefaultTextColor(zaf::Color::Gray());

    styled_text.SetText([&error]() -> std::wstring {
        try {
            std::rethrow_exception(error);
        }
        catch (const std::exception& error) {
            return zaf::FromUTF8String(error.what());
        }
    }());

    auto result = zaf::Create<zaf::TextBox>();
    result->SetMargin(zaf::Frame{ 0, 4, 0, 0 });
    result->SetWordWrapping(zaf::WordWrapping::Wrap);
    result->SetAutoHeight(true);
    result->SetIsEnabled(false);
    result->SetStyledText(std::move(styled_text));
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