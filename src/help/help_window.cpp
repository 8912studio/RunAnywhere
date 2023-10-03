#include "help_window.h"
#include <zaf/base/auto_reset.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/graphic/alignment.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include "help/help_style_config.h"

using namespace ra::help::markdown::render;

namespace ra::help {

ZAF_DEFINE_TYPE(HelpWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///help/help_window.xaml")
ZAF_DEFINE_TYPE_END


void HelpWindow::AfterParse() {

    __super::AfterParse();

    scroll_control_ = zaf::As<zaf::ScrollableControl>(RootControl());
    scroll_control_->SetScrollBarThickness(10);

    auto scroll_bar = scroll_control_->VerticalScrollBar();
    scroll_bar->SetArrowLength(0);
    scroll_bar->SetPadding(zaf::Frame{ 0, 2, 0, 2 });
    scroll_bar->SetSmallChange(22);
    Subscriptions() += scroll_bar->ScrollEvent().Subscribe(std::bind(&HelpWindow::OnScroll, this));

    auto content_layouter = zaf::Create<zaf::VerticalLayouter>();
    content_layouter->SetAxisAlignment(zaf::AxisAlignment::Center);
    scroll_control_->ScrollContent()->SetLayouter(content_layouter);

    InitializeScrollControls();
}


void HelpWindow::InitializeScrollControls() {

    InitializeScrollButtonContainer();
    InitializeScrollButtons();
}


void HelpWindow::InitializeScrollButtonContainer() {

    const auto& root_control = this->RootControl();
    Subscriptions() += root_control->RectChangedEvent().Subscribe(
        std::bind(&HelpWindow::LayoutScrollButtonContainer, this));

    Subscriptions() += root_control->MouseEnterEvent().Subscribe(std::bind([this]() {

        scrollButtonContainer->SetIsVisible(true);
    }));

    Subscriptions() += root_control->MouseLeaveEvent().Subscribe(std::bind([this]() {

        const auto& root_control = this->RootControl();
        if (!root_control->ContainMouse()) {
            scrollButtonContainer->SetIsVisible(false);
        }
    }));
}


void HelpWindow::InitializeScrollButtons() {

    Subscriptions() += lineDownButton->MouseUpEvent().Subscribe(
        std::bind(&HelpWindow::ScrollLine, this, false));

    Subscriptions() += lineUpButton->MouseUpEvent().Subscribe(
        std::bind(&HelpWindow::ScrollLine, this, true));

    Subscriptions() += pageDownButton->MouseUpEvent().Subscribe(
        std::bind(&HelpWindow::ScrollPage, this, false));

    Subscriptions() += pageUpButton->MouseUpEvent().Subscribe(
        std::bind(&HelpWindow::ScrollPage, this, true));
}


void HelpWindow::LayoutScrollButtonContainer() {

    auto scrollable_control = zaf::As<zaf::ScrollableControl>(RootControl());
    auto content_rect = scrollable_control->ContentRect();

    constexpr float margin = 4;

    zaf::Point container_position;

    container_position.x =
        content_rect.position.x + 
        content_rect.size.width - scrollButtonContainer->Width() - 
        scrollable_control->VerticalScrollBarThickness() - 
        margin;

    container_position.y = margin;

    scrollButtonContainer->SetPosition(container_position);
}


void HelpWindow::SetContent(const HelpContent& content) {

    auto auto_reset = zaf::MakeAutoReset(is_setting_content_, true);

    content_id_ = content.ID();
    markdown_region_ = MarkdownRegion::Create(*content.Element(), GetHelpStyleConfig());

    const auto& scroll_content = scroll_control_->ScrollContent();
    scroll_content->RemoveAllChildren();
    scroll_content->AddChild(markdown_region_);
    
    UpdateWindowHeight();

    if (last_scroll_value_ && last_scroll_content_id_ == content_id_) {
        scroll_control_->VerticalScrollBar()->SetValue(*last_scroll_value_);
    }
}


void HelpWindow::UpdateWindowHeight() {

    if (!markdown_region_) {
        return;
    }

    auto preferred_size = markdown_region_->CalculatePreferredSize(zaf::Size{
        this->ContentWidth() - scroll_control_->HorizontalScrollBarThickness(),
        std::numeric_limits<float>::max()
    });

    markdown_region_->SetFixedHeight(preferred_size.height);

    constexpr float max_window_height = 400;
    constexpr float min_window_height = 60;
    constexpr float window_border = 2;

    float actual_window_height{};
    float actual_content_height{};

    float expected_window_height = preferred_size.height + window_border;
    if (expected_window_height < min_window_height) {
        actual_window_height = min_window_height;
        actual_content_height = min_window_height - window_border;
    }
    else if (expected_window_height > max_window_height) {
        actual_window_height = max_window_height;
        actual_content_height = preferred_size.height;
    }
    else {
        actual_window_height = expected_window_height;
        actual_content_height = preferred_size.height;
    }

    const auto& scroll_content = scroll_control_->ScrollContent();
    scroll_content->SetFixedHeight(actual_content_height);

    this->SetHeight(std::ceil(actual_window_height));
}


void HelpWindow::OnScroll() {

    if (!is_setting_content_) {
        last_scroll_content_id_ = content_id_;
        last_scroll_value_ = scroll_control_->VerticalScrollBar()->Value();
    }
}


void HelpWindow::ScrollLine(bool scroll_up) {

    auto& scrollable_control = zaf::As<zaf::ScrollableControl>(*RootControl());
    auto scroll_bar = scrollable_control.VerticalScrollBar();

    int small_change = scroll_bar->SmallChange();
    int new_value = scroll_bar->Value() + (scroll_up ? -small_change : small_change);
    scroll_bar->SetValue(new_value);
}


void HelpWindow::ScrollPage(bool scroll_up) {

    auto& scrollable_control = zaf::As<zaf::ScrollableControl>(*RootControl());
    auto scroll_bar = scrollable_control.VerticalScrollBar();

    int page_size = scroll_bar->PageSize();
    int new_value = scroll_bar->Value() + (scroll_up ? -page_size : page_size);
    scroll_bar->SetValue(new_value);
}

}