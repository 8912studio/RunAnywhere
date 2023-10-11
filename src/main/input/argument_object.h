#pragma once

#include <zaf/base/none.h>
#include <zaf/control/rich_edit/embedded_object.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include <zaf/window/window.h>
#include "main/command_display_style.h"
#include "main/input/argument_data.h"
#include "main/input/argument_object_window.h"

namespace ra::main::input {

class ArgumentObject : public zaf::rich_edit::EmbeddedObject, zaf::SubscriptionHost {
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

    void Paint(
        zaf::Canvas& canvas,
        const zaf::Rect& dirty_rect,
        const zaf::rich_edit::PaintContext& context) override;

    void OnMouseCursorChanging(const zaf::rich_edit::MouseCursorChangingContext& context) override;
    bool OnDoubleClick(const zaf::rich_edit::DoubleClickContext& context) override;

protected:
    explicit ArgumentObject(std::shared_ptr<ArgumentData> data);

    virtual std::shared_ptr<ArgumentData> CreateData(std::wstring text) = 0;
    virtual zaf::Color GetBackgroundColor(const zaf::rich_edit::PaintContext& context) = 0;
    virtual std::shared_ptr<ArgumentObjectWindow> CreateArgumentObjectWindow() = 0;

private:
    void PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect);
    bool InnerOpenWindow(const zaf::Point& object_position_in_screen);
    void OnTextChanged(const std::shared_ptr<ArgumentObjectWindow>& window);
    void OnWindowDestroyed();

private:
    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
    std::shared_ptr<ArgumentData> data_;
    zaf::Subject<zaf::None> text_changed_event_;
};

}