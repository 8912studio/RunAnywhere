#include "module/chat_gpt/chat_gpt_answer_view.h"
#include "help/help_style_config.h"
#include "utility/markdown/parse/markdown_parser.h"

using namespace ra::utility::markdown::parse;
using namespace ra::utility::markdown::render;

namespace ra::mod::chat_gpt {

void ChatGPTAnswerView::SetAnswer(const std::wstring& answer) {

    auto root_element = MarkdownParser::Instance()->Parse(answer);
    markdown_region_ = MarkdownRegion::Create(*root_element, help::GetHelpStyleConfig());

    this->AddChild(markdown_region_);
    ResetHeight();
}


void ChatGPTAnswerView::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    if (event_info.PreviousRect().size.width != this->Width()) {
        ResetHeight();
    }
}


void ChatGPTAnswerView::ResetHeight() {

    if (!markdown_region_) {
        return;
    }

    auto preferred_size = markdown_region_->CalculatePreferredSize(this->ContentSize());
    this->SetFixedHeight(std::ceil(preferred_size.height));
}

}