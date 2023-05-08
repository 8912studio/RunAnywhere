#include "main/text_block_object.h"
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include "utility/text_utility.h"

namespace ra {

TextBlockObject::TextBlockObject(const std::wstring& text) : text_(text) {
    this->SetSize(zaf::Size{ 64, 28 });
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
    rounded_rect.rect.Deflate(zaf::Frame{ 2, 0, 2, 0 });
    rounded_rect.x_radius = 4;
    rounded_rect.y_radius = 4;

    auto background_color = 
        context.IsInSelectionRange() ? 
        zaf::Color::FromARGB(0x7d2EAFFF) : 
        zaf::Color::FromARGB(0x7d95D4FF);

    canvas.DrawRoundedRectangle(
        rounded_rect,
        canvas.Renderer().CreateSolidColorBrush(background_color));

    PaintText(canvas, rounded_rect.rect);
}


void TextBlockObject::PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect) {

    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_size = 16;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::TextTrimming text_trimming;
    text_trimming.granularity = zaf::TextTrimming::Granularity::Character;
    text_trimming.trimming_sign = 
        zaf::GraphicFactory::Instance().CreateEllipsisTrimmingSign(text_format);
    text_format.SetTextTrimming(text_trimming);

    text_format.SetTextAlignment(zaf::TextAlignment::Center);
    text_format.SetParagraphAlignment(zaf::ParagraphAlignment::Center);

    zaf::Rect paint_rect = text_rect;
    paint_rect.Deflate(zaf::Frame{ 4, 0, 3, 0 });

    canvas.DrawTextFormat(
        utility::RemoveLineBreaks(text_),
        text_format,
        paint_rect,
        canvas.Renderer().CreateSolidColorBrush(zaf::Color::Black()));
}


void TextBlockObject::OnMouseCursorChanging(
    const zaf::rich_edit::MouseCursorChangingContext& context) {

    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    context.EventInfo().MarkAsHandled();
}

}