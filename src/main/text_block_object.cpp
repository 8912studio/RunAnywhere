#include "main/text_block_object.h"
#include <tom.h>
#include <zaf/base/as.h>
#include <zaf/creation.h>
#include <zaf/control/rich_edit.h>
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include "utility/text_utility.h"

namespace ra {

TextBlockObject::TextBlockObject(std::shared_ptr<TextBlockData> data) : data_(std::move(data)) {

    this->SetSize(zaf::Size{ 60, 28 });
}


TextBlockObject::TextBlockObject(std::wstring text) : 
    TextBlockObject(std::make_shared<TextBlockData>(std::move(text))) {

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
    rounded_rect.rect.Deflate(zaf::Frame{ 2, 1, 2, 1 });
    rounded_rect.x_radius = 4;
    rounded_rect.y_radius = 4;

    auto background_color = 
        context.IsInSelectionRange() ? 
        zaf::Color::FromARGB(0x7d2EAFFF) : 
        zaf::Color::FromARGB(0x7d95D4FF);

    canvas.DrawRoundedRectangle(rounded_rect, background_color);

    PaintText(canvas, rounded_rect.rect);
}


void TextBlockObject::PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect) {

    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_size = 16;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::TextTrimming text_trimming;
    text_trimming.SetGranularity(zaf::TextTrimmingGranularity::Character);
    text_trimming.SetSign(zaf::GraphicFactory::Instance().CreateEllipsisTrimmingSign(text_format));
    text_format.SetTextTrimming(text_trimming);

    text_format.SetTextAlignment(zaf::TextAlignment::Center);
    text_format.SetParagraphAlignment(zaf::ParagraphAlignment::Center);

    zaf::Rect paint_rect = text_rect;
    paint_rect.Deflate(zaf::Frame{ 4, 0, 3, 0 });

    canvas.DrawTextFormat(
        utility::RemoveLineBreaks(Text()), 
        text_format, 
        paint_rect,
        zaf::Color::Black());
}


void TextBlockObject::OnMouseCursorChanging(
    const zaf::rich_edit::MouseCursorChangingContext& context) {

    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    context.EventInfo().MarkAsHandled();
}


bool TextBlockObject::OnDoubleClick(const zaf::rich_edit::DoubleClickContext& context) {
    return InnerOpenWindow(context.ObjectPositionInScreen());
}


void TextBlockObject::OpenWindow() {

    try {

        auto position = this->GetPositionInScreen();
        if (position) {
            InnerOpenWindow(*position);
        }
    }
    catch (const zaf::Error&) {

    }
}


bool TextBlockObject::InnerOpenWindow(const zaf::Point& object_position_in_screen) {

    auto host = Host();
    if (!host) {
        return false;
    }

    auto host_window = host->Window();
    if (!host_window) {
        return false;
    }

    auto window = zaf::Create<TextBlockWindow>();
    window->SetOwner(host_window);
    window->SetInitialRectStyle(zaf::InitialRectStyle::Custom);

    window->SetObjectPositionInScreen(object_position_in_screen);
    window->SetText(Text());

    Subscriptions() += window->TextChangedEvent().Subscribe(
        std::bind(&TextBlockObject::OnTextChanged, this, std::placeholders::_1));

    Subscriptions() += window->DestroyedEvent().Subscribe(
        std::bind(&TextBlockObject::OnWindowDestroyed, this));

    window->Show();
    return true;
}


void TextBlockObject::OnTextChanged(const std::shared_ptr<TextBlockWindow>& window) {

    auto new_text = window->GetText();

    //Repaint only if several head chars are different. 
    constexpr std::size_t CompareCount = 10;
    bool need_repaint = new_text.compare(0, CompareCount, Text(), 0, CompareCount) != 0;

    data_ = std::make_shared<TextBlockData>(std::move(new_text));

    if (need_repaint) {
        this->NeedRepaint();
    }

    text_changed_event_.GetObserver().OnNext({});
}


void TextBlockObject::OnWindowDestroyed() {

    //Remove current object from rich edit if text is empty.
    if (!Text().empty()) {
        return;
    }
    
    try {

        auto char_index = this->GetCharIndex();
        if (!char_index) {
            return;
        }

        auto host = Host();
        if (!host) {
            return;
        }

        auto text_document = host->GetOLEInterface().Query<ITextDocument>();
        if (!text_document) {
            return;
        }

        zaf::COMObject<ITextRange> text_range;
        HRESULT hresult = text_document->Range(
            static_cast<long>(*char_index),
            static_cast<long>(*char_index) + 1, 
            text_range.Reset());

        if (FAILED(hresult)) {
            return;
        }

        hresult = text_range->Delete(tomCharacter, 0, nullptr);
        if (FAILED(hresult)) {
            return;
        }
    }
    catch (const zaf::Error&) {

    }
}

}