#include "help_window.h"
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/graphic/alignment.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include "help/help_style_config.h"
#include "help/markdown/render/markdown_region.h"

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
    scroll_bar->SetSmallChange(16);

    auto scroll_content = scroll_control_->ScrollContent();
    scroll_content->SetLayouter(zaf::Create<zaf::VerticalLayouter>());

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


void HelpWindow::SetContent(const markdown::element::Element& content) {

    auto region = MarkdownRegion::Create(content, GetHelpStyleConfig());
    InstallHelpContent(region);
}


void HelpWindow::InstallHelpContent(const std::shared_ptr<zaf::Control>& control) {

    const auto& scroll_content = scroll_control_->ScrollContent();
    scroll_content->RemoveAllChildren();
    scroll_content->AddChild(control);

    help_content_control_ = control;
    UpdateWindowHeight();
}


void HelpWindow::UpdateWindowHeight() {

    if (!help_content_control_) {
        return;
    }

    auto preferred_size = help_content_control_->CalculatePreferredSize(zaf::Size{
        this->ContentWidth(),
        std::numeric_limits<float>::max()
    });

    const auto& scroll_content = scroll_control_->ScrollContent();
    scroll_content->SetFixedHeight(preferred_size.height);

    //Set window height at next message loop to avoid re-enter issues.
    Subscriptions() += zaf::rx::Just(0).SubscribeOn(zaf::Scheduler::Main()).Subscribe(
        [this, preferred_size](int) {

        constexpr float max_height = 400;
        constexpr float min_height = 60;

        constexpr float window_border = 2;
        float expected_height = preferred_size.height + window_border;

        float window_height = std::min(max_height, expected_height);
        window_height = std::max(window_height, min_height);

        this->SetHeight(std::ceil(window_height));
    });
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