#pragma once

#include <zaf/base/none.h>
#include <zaf/control/rich_edit/embedded_object.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include <zaf/window/window.h>
#include "main/text_block_window.h"

namespace ra {

class TextBlockObject : public zaf::rich_edit::EmbeddedObject, zaf::SubscriptionHost {
public:
    explicit TextBlockObject(const std::wstring& text);

    const std::wstring& Text() const {
        return text_;
    }

    zaf::Observable<zaf::None> TextChangedEvent() {
        return text_changed_event_.GetObservable();
    }

    GUID ClassID() const override;

    void Paint(
        zaf::Canvas& canvas,
        const zaf::Rect& dirty_rect,
        const zaf::rich_edit::PaintContext& context) override;

    void OnMouseCursorChanging(const zaf::rich_edit::MouseCursorChangingContext& context) override;
    bool OnDoubleClick(const zaf::rich_edit::DoubleClickContext& context) override;

private:
    void PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect);
    void OnTextChanged(const std::shared_ptr<TextBlockWindow>& window);
    void OnWindowDestroyed();

private:
    std::wstring text_;
    zaf::Subject<zaf::None> text_changed_event_;
};

}