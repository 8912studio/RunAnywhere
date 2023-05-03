#pragma once

#include <zaf/control/rich_edit/embedded_object.h>

namespace ra {

class TextBlockObject : public zaf::rich_edit::EmbeddedObject {
public:
    explicit TextBlockObject(const std::wstring& text);

    GUID ClassID() const override;

    void Paint(
        zaf::Canvas& canvas,
        const zaf::Rect& dirty_rect,
        const zaf::rich_edit::PaintContext& context) override;

    void OnMouseCursorChanging(const zaf::rich_edit::MouseCursorChangingContext& context) override;

private:
    std::wstring text_;
};

}