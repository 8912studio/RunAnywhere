#include "main/text_block_object.h"
#include <zaf/graphic/canvas.h>

namespace ra {

TextBlockObject::TextBlockObject(const std::wstring& text) : text_(text) {

    this->SetSize(zaf::Size{ 60, 28 });
}


GUID TextBlockObject::ClassID() const {
    return { 0xb472b6af, 0xe996, 0x4120, { 0xab, 0xc8, 0x97, 0xa8, 0xf8, 0x65, 0x3d, 0x3b } };
}


void TextBlockObject::Paint(
    zaf::Canvas& canvas,
    const zaf::Rect& dirty_rect,
    const zaf::rich_edit::PaintContext& context) {

    zaf::RoundedRect rounded_rect;
    rounded_rect.rect.size = this->Size();
    rounded_rect.rect.Deflate(zaf::Frame{ 2 });
    rounded_rect.x_radius = 4;
    rounded_rect.y_radius = 4;

    canvas.DrawRoundedRectangle(
        rounded_rect,
        canvas.Renderer().CreateSolidColorBrush(zaf::Color::Blue()));
}


void TextBlockObject::OnMouseCursorChanging(
    const zaf::rich_edit::MouseCursorChangingContext& context) {

    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    context.EventInfo().MarkAsHandled();
}

}