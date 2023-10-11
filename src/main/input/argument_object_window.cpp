#include "main/input/argument_object_window.h"
#include <zaf/base/as.h>
#include <zaf/base/none.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>

namespace ra::main::input {

void ArgumentObjectWindow::SetObjectPositionInScreen(const zaf::Point& position) {
    object_position_in_screen_ = position;
}


zaf::Observable<std::shared_ptr<ArgumentObjectWindow>> ArgumentObjectWindow::TextChangedEvent() {
    return text_changed_event_.AsObservable();
}


void ArgumentObjectWindow::RaiseTextChangedEvent() {
    text_changed_event_.AsObserver().OnNext(zaf::As<ArgumentObjectWindow>(shared_from_this()));
}


void ArgumentObjectWindow::Relayout() {

    constexpr float WindowHorizontalBorder = 2;
    constexpr float WindowVerticalBorder = 2;
    constexpr float MinWindowWidth = 320.f;

    auto content_size = CalculateWindowContentSize();

    zaf::Size window_size{
        WindowHorizontalBorder + content_size.width,
        WindowVerticalBorder + content_size.height,
    };

    window_size.width = std::max(window_size.width, MinWindowWidth);

    zaf::Rect window_rect{ object_position_in_screen_, window_size };
    window_rect.position.y -= window_size.height + 4;
    this->SetRect(window_rect);
}


void ArgumentObjectWindow::OnShow(const zaf::ShowInfo& event_info) {

    __super::OnShow(event_info);

    SetFocus(this->Handle());
}


void ArgumentObjectWindow::OnDeactivated(const zaf::DeactivatedInfo& event_info) {

    __super::OnDeactivated(event_info);

    //Close window at next message loop to avoid focus issues.
    //TODO: Need more elegant method to schedule task at next message loop.
    Subscriptions() += zaf::rx::Create<zaf::None>([this](zaf::Observer<zaf::None>) {
        this->Close();
        return zaf::Subscription{};
    })
    .SubscribeOn(zaf::Scheduler::Main())
    .Subscribe();
}


void ArgumentObjectWindow::OnMessageReceived(const zaf::MessageReceivedInfo& event_info) {

    switch (event_info.Message().ID()) {
    case WM_KEYDOWN: {
        if (zaf::KeyMessage(event_info.Message()).VirtualKey() == VK_ESCAPE) {
            this->Close();
            event_info.MarkAsHandled(0);
        }
        break;
    }
    default:
        break;
    }

    __super::OnMessageReceived(event_info);
}

}