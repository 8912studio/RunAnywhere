#include "utility/markdown/render/code_block_region.h"
#include <zaf/base/log.h>
#include <zaf/base/error/contract_error.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/creation.h>
#include "utility/clipboard.h"
#include "utility/markdown/element/code_block_element.h"

namespace ra::utility::markdown::render {

ZAF_OBJECT_IMPL(CodeBlockRegion);

std::shared_ptr<CodeBlockRegion> CodeBlockRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::CodeBlock);
    const auto& code_block_element = dynamic_cast<const element::CodeBlockElement&>(element);

    auto result = zaf::Init(new CodeBlockRegion());
    if (!code_block_element.Children().empty()) {

        zaf::textual::TextStyle text_style;
        text_style.SetFont(style_config.basic_config.font);
        text_style.Font()->family_name = 
            style_config.code_block_config.code_config.font_family_name;
        text_style.SetTextColor(style_config.code_block_config.code_config.text_color);

        result->SetStyledText(code_block_element.Children().front()->Text(), text_style);
    }
    result->SetShowHeader(style_config.code_block_config.show_header);
    result->SetLanguage(code_block_element.Language());
    result->SetTextBackgroundColor(style_config.code_block_config.code_config.background_color);
    return result;
}


void CodeBlockRegion::AfterParse() {

    __super::AfterParse();

    Subscriptions() += copyButton->ClickEvent().Subscribe(std::bind([this]() {
         utility::SetStringToClipboard(textBox->Text());
    }));
}


zaf::Size CodeBlockRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto result = textBox->CalculatePreferredSize(bound_size);
    if (result.width > bound_size.width) {
        result.height += scrollControl->HorizontalScrollBar()->Height();
    }

    result.height += scrollControl->Margin().Height();

    if (header->IsVisible()) {
        result.height += header->Height() + header->Margin().Height();
    }
    return result;
}


void CodeBlockRegion::OnMouseEnter(const zaf::MouseEnterInfo& event_info) {

    __super::OnMouseEnter(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    copyButton->SetIsVisible(true);
}


void CodeBlockRegion::OnMouseLeave(const zaf::MouseLeaveInfo& event_info) {

    __super::OnMouseLeave(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!this->ContainsMouse()) {
        copyButton->SetIsVisible(false);
    }
}


void CodeBlockRegion::SetStyledText(
    const std::wstring& text, 
    const zaf::textual::TextStyle& text_style) {

    textBox->SetText(text);

    if (auto font = text_style.Font()) {
        textBox->SetFont(*font);
    }

    if (auto text_color = text_style.TextColor()) {
        textBox->SetTextColor(*text_color);
    }
}


void CodeBlockRegion::SetShowHeader(bool show) {
    header->SetIsVisible(show);
}


void CodeBlockRegion::SetLanguage(const std::wstring& language) {

    if (!language.empty()) {
        languageLabel->SetText(language);
    }
    else {
        languageLabel->SetText(L"text");
    }
}


void CodeBlockRegion::SetTextBackgroundColor(const zaf::Color& color) {

    this->SetBackgroundColor(color);
    textBox->SetBackgroundColor(color);
}


bool CodeBlockRegion::IsPositionInsideTextBoundary(const zaf::Point& mouse_position) {

    auto position_in_scroll = scrollControl->TranslateFromParent(mouse_position);
    auto position_in_text_box = scrollControl->TranslateToScrollContent(position_in_scroll);

    if (textBox->RectInSelf().Contain(position_in_text_box)) {
        return textBox->IsPositionInsideText(position_in_text_box);
    }

    return false;
}


void CodeBlockRegion::BeginSelection(const zaf::Point& position) {

    if (!IsPositionInTextBox(position)) {
        return;
    }

    auto horizontal_scroll_bar = scrollControl->HorizontalScrollBar();
    begin_selection_x_offset_ = static_cast<float>(horizontal_scroll_bar->Value());
}


void CodeBlockRegion::ChangeSelection(const composite::PositionRange& position_range) {

    auto begin_position_in_scroll = scrollControl->TranslateFromParent(position_range.Begin());
    auto begin_position_in_text_box = 
        scrollControl->TranslateToScrollContent(begin_position_in_scroll);

    auto end_position_in_scroll = scrollControl->TranslateFromParent(position_range.End());
    auto end_position_in_text_box = 
        scrollControl->TranslateToScrollContent(end_position_in_scroll);

    bool scroll_to_selection{ false };
    if (begin_selection_x_offset_) {

        auto horizontal_scroll_bar = scrollControl->HorizontalScrollBar();
        auto current_x_offset = static_cast<float>(horizontal_scroll_bar->Value());

        begin_position_in_text_box.x -= current_x_offset - *begin_selection_x_offset_ ;

        //If both begin and end position are in the rectangle of the text box, scroll to the 
        //selection.
        if (IsPositionInTextBox(position_range.End())) {
            scroll_to_selection = true;
        }
    }

    composite::PositionRange position_range_in_text_box{ 
        begin_position_in_text_box,
        end_position_in_text_box,
    };
    textBox->SetSelectionByPositionRange(position_range_in_text_box, scroll_to_selection);
}


bool CodeBlockRegion::IsPositionInTextBox(const zaf::Point& position) const {

    auto position_in_scroll = scrollControl->TranslateFromParent(position);
    auto position_in_text_box = scrollControl->TranslateToScrollContent(position_in_scroll);

    return 
        (position_in_text_box.y >= 0) && 
        (position_in_text_box.y < textBox->Height());
}


void CodeBlockRegion::EndSelection() {
    begin_selection_x_offset_.reset();
}


void CodeBlockRegion::SelectWord(const zaf::Point& position) {

    if (!IsPositionInTextBox(position)) {
        return;
    }

    auto position_in_scroll = scrollControl->TranslateFromParent(position);
    auto position_in_text_box = scrollControl->TranslateToScrollContent(position_in_scroll);
    textBox->SelectWord(position_in_text_box);
}


void CodeBlockRegion::BuildSelectedText(composite::SelectedTextBuilder& builder) {
    textBox->BuildSelectedText(builder);
}


void CodeBlockRegion::ChangeFocus(bool is_focused) {
    textBox->ChangeFocus(is_focused);
}

}