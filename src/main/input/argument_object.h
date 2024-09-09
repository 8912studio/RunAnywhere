#pragma once

#include <zaf/base/none.h>
#include <zaf/control/textual/interactive_inline_object.h>
#include <zaf/rx/subject.h>
#include <zaf/window/window.h>
#include "main/command_display_style.h"
#include "main/input/argument_data.h"
#include "main/input/argument_object_window.h"

namespace ra::main::input {

class ArgumentObject : public zaf::textual::InteractiveInlineObject {
public:
    const std::shared_ptr<ArgumentData>& Data() const {
        return data_;
    }

    const std::wstring& Text() const {
        return data_->Text();
    }

    void SetStyle(CommandDisplayStyle style);

    void OpenWindow();

    zaf::Observable<zaf::None> TextChangedEvent() {
        return text_changed_event_.AsObservable();
    }

    zaf::TextInlineObjectMetrics GetMetrics() const override;

protected:
    explicit ArgumentObject(std::shared_ptr<ArgumentData> data);

    void Paint(zaf::Canvas& canvas) const override;

    void OnMouseCursorChanging(
        const zaf::textual::InlineObjectMouseCursorChangingInfo& event_info) override;
    void OnMouseDown(const zaf::textual::InlineObjectMouseDownInfo& event_info) override;
    void OnDoubleClick(const zaf::textual::InlineObjectDoubleClickInfo& event_info) override;

    virtual std::shared_ptr<ArgumentData> CreateData(std::wstring text) = 0;
    virtual zaf::Color GetBackgroundColor() const = 0;
    virtual std::shared_ptr<ArgumentObjectWindow> CreateArgumentObjectWindow() = 0;

private:
    void PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect) const;
    bool InnerOpenWindow();
    void OnTextChanged(const std::shared_ptr<ArgumentObjectWindow>& window);
    void OnWindowDestroyed();

private:
    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
    std::shared_ptr<ArgumentData> data_;
    zaf::Subject<zaf::None> text_changed_event_;
};

}