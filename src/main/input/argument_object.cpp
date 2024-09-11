#include "main/input/argument_object.h"
#include <tom.h>
#include <zaf/base/as.h>
#include <zaf/control/text_box.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/dwrite/text_format_properties.h>
#include "utility/text_utility.h"

namespace ra::main::input {

ArgumentObject::ArgumentObject(std::shared_ptr<ArgumentData> data) : data_(std::move(data)) {

    SetStyle(CommandDisplayStyle::Normal);
}


void ArgumentObject::SetStyle(CommandDisplayStyle style) {
    style_ = style;
    this->NeedRepaint();
}


zaf::dwrite::InlineObjectMetrics ArgumentObject::GetMetrics() const {

    zaf::dwrite::InlineObjectMetrics result;
    if (style_ == CommandDisplayStyle::Preserved) {
        result.SetWidth(48);
        result.SetHeightAndBaseline(18);
    }
    else {
        result.SetWidth(60);
        result.SetHeightAndBaseline(28);
    }
    return result;
}


void ArgumentObject::Paint(zaf::Canvas& canvas) const {

    zaf::RoundedRect rounded_rect;
    rounded_rect.rect.size = this->Size();
    rounded_rect.rect.Deflate(zaf::Frame{ 2, 1, 2, 1 });
    rounded_rect.x_radius = 4;
    rounded_rect.y_radius = 4;

    auto background_color = GetBackgroundColor();
    canvas.DrawRoundedRectangle(rounded_rect, background_color);

    PaintText(canvas, rounded_rect.rect);
}


void ArgumentObject::PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect) const {

    zaf::dwrite::TextFormatProperties text_format_properties;
    text_format_properties.font_size = style_ == CommandDisplayStyle::Preserved ? 12.f : 16.f;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::dwrite::TextTrimming text_trimming;
    text_trimming.SetGranularity(zaf::dwrite::TextTrimmingGranularity::Character);
    text_trimming.SetSign(zaf::GraphicFactory::Instance().CreateEllipsisTrimmingSign(text_format));
    text_format.SetTextTrimming(text_trimming);

    text_format.SetWordWrapping(zaf::dwrite::WordWrapping::NoWrap);
    text_format.SetTextAlignment(zaf::dwrite::TextAlignment::Center);
    text_format.SetParagraphAlignment(zaf::dwrite::ParagraphAlignment::Center);

    zaf::Rect paint_rect = text_rect;
    paint_rect.Deflate(zaf::Frame{ 4, 0, 3, 0 });

    canvas.DrawTextFormat(
        utility::ReplaceWhitespacesToVisibleChars(Text()),
        text_format,
        paint_rect,
        zaf::Color::Black());
}


void ArgumentObject::OnMouseCursorChanging(
    const zaf::textual::InlineObjectMouseCursorChangingInfo& event_info) {

    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    event_info.MarkAsHandled();
}


void ArgumentObject::OnMouseDown(const zaf::textual::InlineObjectMouseDownInfo& event_info) {

    auto host = Host();
    if (!host) {
        return;
    }

    auto range_in_host = RangeInHost();
    if (!range_in_host) {
        return;
    }

    host->SetSelectionRange(*range_in_host);
    event_info.MarkAsHandled();
}


void ArgumentObject::OnDoubleClick(const zaf::textual::InlineObjectDoubleClickInfo& event_info) {
    if (InnerOpenWindow()) {
        event_info.MarkAsHandled();
    }
}


void ArgumentObject::OpenWindow() {
    InnerOpenWindow();
}


bool ArgumentObject::InnerOpenWindow() {

    try {

        auto host = Host();
        if (!host) {
            return false;
        }

        auto host_window = host->Window();
        if (!host_window) {
            return false;
        }

        auto position_in_window = *this->PositionInHost();
        position_in_window.AddOffset(*host->PositionInWindow());
        auto position_in_screen = host_window->TranslateToScreen(position_in_window);

        auto window = CreateArgumentObjectWindow();
        window->SetOwner(host_window);
        window->SetInitialRectStyle(zaf::InitialRectStyle::Custom);

        window->SetObjectPositionInScreen(position_in_screen);
        window->SetIsReadOnly(style_ != CommandDisplayStyle::Normal);
        window->SetText(Text());

        Subscriptions() += window->TextChangedEvent().Subscribe(
            std::bind(&ArgumentObject::OnTextChanged, this, std::placeholders::_1));

        Subscriptions() += window->DestroyedEvent().Subscribe(
            std::bind(&ArgumentObject::OnWindowDestroyed, this));

        window->Show();
        return true;
    }
    catch (...) {
        return false;
    }
}


void ArgumentObject::OnTextChanged(const std::shared_ptr<ArgumentObjectWindow>& window) {

    auto new_text = window->GetText();

    //Repaint only if several head chars are different. 
    constexpr std::size_t CompareCount = 10;
    bool need_repaint = new_text.compare(0, CompareCount, Text(), 0, CompareCount) != 0;

    data_ = CreateData(std::move(new_text));

    if (need_repaint) {
        this->NeedRepaint();
    }

    text_changed_event_.AsObserver().OnNext({});
}


void ArgumentObject::OnWindowDestroyed() {

    //Remove current object from rich edit if text is empty.
    if (!Text().empty()) {
        return;
    }

    try {

        auto range_in_host = this->RangeInHost();
        if (!range_in_host) {
            return;
        }

        auto host = Host();
        if (!host) {
            return;
        }

        host->SetTextInRange({}, *range_in_host);
    }
    catch (...) {

    }
}

}