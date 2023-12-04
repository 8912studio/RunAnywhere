#include "module/common/binary_content/binary_content_control.h"
#include <zaf/control/scroll_bar.h>
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>

namespace ra::mod {

ZAF_DEFINE_TYPE(BinaryContentControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/binary_content/binary_content_control.xaml")
ZAF_DEFINE_TYPE_END;

void BinaryContentControl::AfterParse() {

    __super::AfterParse();

    auto scroll_bar = scrollControl->VerticalScrollBar();
    scroll_bar->SetSmallChange(static_cast<int>(LineHeight));

    //If there is too much data, cached painting will fail, so we have to disable it.
    scrollControl->ScrollContent()->SetIsCachedPaintingEnabled(false);
}


void BinaryContentControl::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    canvas.DrawRectangle(
        zaf::Rect{ 0, 0, LineHeaderWidth, this->ContentSize().height },
        HeaderBackgroundColor());
}


const std::vector<std::byte>& BinaryContentControl::GetBinary() const {
    return body->GetBinary();
}


void BinaryContentControl::SetBinary(std::vector<std::byte> binary) {

    body->SetBinary(std::move(binary));
    AdjustBodyHeight();
}


void BinaryContentControl::ChangeStyle(CommandDisplayStyle style) {

    style_ = style;
    AdjustBodyHeight();
}


void BinaryContentControl::AdjustBodyHeight() {

    float body_height = body->CalculatePreferredSize().height;
    if (style_ == CommandDisplayStyle::Preserved) {
        scrollControl->SetFixedHeight(body_height);
        return;
    }

    //LineHeight is the header.
    constexpr float min_height = 90 - LineHeight;
    
    constexpr int lines_per_page = 8;
    constexpr float max_height = LineHeight * lines_per_page;

    float scroll_height = std::max(body_height, min_height);
    scroll_height = std::min(scroll_height, max_height);
    scrollControl->SetFixedHeight(scroll_height);
}

}