#include "module/chat_gpt/chat_gpt_preview_control.h"
#include <zaf/graphic/alignment.h>
#include <zaf/object/type_definition.h>
#include "help/help_style_config.h"
#include "utility/markdown/parse/markdown_parser.h"

using namespace ra::utility::markdown::parse;
using namespace ra::utility::markdown::render;

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTPreviewControl::SetAnswer(const std::wstring& answer) {

    this->RemoveAllChildren();

    auto root_element = MarkdownParser::Instance()->Parse(answer);
    markdown_region_ = MarkdownRegion::Create(*root_element, help::GetHelpStyleConfig());
    this->AddChild(markdown_region_);

    ResetHeight();
}


void ChatGPTPreviewControl::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    if (event_info.PreviousRect().size.width != this->Width()) {
        ResetHeight();
    }
}


void ChatGPTPreviewControl::ResetHeight() {

    if (markdown_region_) {
        auto preferred_size = markdown_region_->CalculatePreferredSize(this->ContentSize());
        this->SetFixedHeight(std::ceil(preferred_size.height));
    }
}


zaf::Frame ChatGPTPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 5;
    return result;
}

}