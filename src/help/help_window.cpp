#include "help_window.h"
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/graphic/alignment.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>

namespace ra::help {

ZAF_DEFINE_TYPE(HelpWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///help/help_window.xaml")
ZAF_DEFINE_TYPE_END


void HelpWindow::AfterParse() {

    __super::AfterParse();

    content_control_ = zaf::Create<HelpContentControl>();
    Subscriptions() += content_control_->NeedUpdateHeightEvent().Subscribe(
        std::bind(&HelpWindow::HelpWindow::OnNeedUpdateHeight, this, std::placeholders::_1));

    auto& scrollable_control = zaf::As<zaf::ScrollableControl>(*RootControl());
    scrollable_control.SetScrollBarThickness(15);

    scrollable_control.GetVerticalScrollBar()->SetSmallChange(16);

    auto scroll_content_control = scrollable_control.ScrollContentControl();
    scroll_content_control->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    scroll_content_control->AddChild(content_control_);
}


void HelpWindow::OnNeedUpdateHeight(float new_height) {

    auto& scrollable_control = zaf::As<zaf::ScrollableControl>(*RootControl());
    scrollable_control.SetScrollContentSize(zaf::Size{ 0, new_height });

    //Set window height at next message loop to avoid re-enter issues.
    Subscriptions() += zaf::rx::Just(0).SubscribeOn(zaf::Scheduler::Main()).Subscribe(
        [this, new_height](int) {

        constexpr float max_height = 300;
        constexpr float min_height = 60;

        constexpr float window_border = 2;
        float expected_height = new_height + window_border;

        float window_height = std::min(max_height, expected_height);
        window_height = std::max(window_height, min_height);

        this->SetHeight(std::ceil(window_height));
    });
}


void HelpWindow::SetContent(const content::Content& content) {

    content_control_->SetContent(content);
}


void HelpWindow::ScrollLine(bool scroll_up) {

    auto& scrollable_control = zaf::As<zaf::ScrollableControl>(*RootControl());
    auto scroll_bar = scrollable_control.GetVerticalScrollBar();

    int small_change = scroll_bar->GetSmallChange();
    int new_value = scroll_bar->GetValue() + (scroll_up ? -small_change : small_change);
    scroll_bar->SetValue(new_value);
}


void HelpWindow::ScrollPage(bool scroll_up) {

    auto& scrollable_control = zaf::As<zaf::ScrollableControl>(*RootControl());
    auto scroll_bar = scrollable_control.GetVerticalScrollBar();

    int page_size = scroll_bar->GetPageSize();
    int new_value = scroll_bar->GetValue() + (scroll_up ? -page_size : page_size);
    scroll_bar->SetValue(new_value);
}


}