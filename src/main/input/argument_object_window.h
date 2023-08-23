#pragma once

#include <zaf/rx/subject.h>
#include "utility/thin_border_window.h"

namespace ra::main::input {

class ArgumentObjectWindow : public utility::ThinBorderWindow {
public:
    void SetObjectPositionInScreen(const zaf::Point& position);
    zaf::Observable<std::shared_ptr<ArgumentObjectWindow>> TextChangedEvent();

    virtual void SetIsReadOnly(bool is_read_only) = 0;
    virtual std::wstring GetText() = 0;
    virtual void SetText(const std::wstring& text) = 0;

protected:
    void RaiseTextChangedEvent();
    void Relayout();

    virtual zaf::Size CalculateWindowContentSize() = 0;

private:
    zaf::Point object_position_in_screen_;
    zaf::Subject<std::shared_ptr<ArgumentObjectWindow>> text_changed_event_;
};

}