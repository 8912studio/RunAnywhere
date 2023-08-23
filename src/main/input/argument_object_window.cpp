#include "main/input/argument_object_window.h"
#include <zaf/base/as.h>

namespace ra::main::input {

void ArgumentObjectWindow::SetObjectPositionInScreen(const zaf::Point& position) {
    object_position_in_screen_ = position;
}


zaf::Observable<std::shared_ptr<ArgumentObjectWindow>> ArgumentObjectWindow::TextChangedEvent() {
    return text_changed_event_.GetObservable();
}


void ArgumentObjectWindow::RaiseTextChangedEvent() {
    text_changed_event_.GetObserver().OnNext(zaf::As<ArgumentObjectWindow>(shared_from_this()));
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

}