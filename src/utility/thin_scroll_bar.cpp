#include "utility/thin_scroll_bar.h"
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>

namespace ra::utility {
namespace {

class ThinScrollBarThumb : public zaf::ScrollBarThumb {
protected:
    void Paint(zaf::Canvas& canvas, const zaf::Rect&) const override {

        if (!IsEnabledInContext()) {
            return;
        }

        constexpr float PaintThickness = 6;
        constexpr float Margin = 2;

        auto content_rect = ContentRect();

        zaf::Rect paint_rect;
        if (IsHorizontal()) {
            paint_rect.position.x = content_rect.position.x + Margin;
            paint_rect.position.y = 
                content_rect.position.y + content_rect.size.height - PaintThickness;
            paint_rect.size.width = content_rect.size.width - Margin * 2;
            paint_rect.size.height = PaintThickness;
        }
        else {
            paint_rect.position.x = 
                content_rect.position.x + content_rect.size.width - PaintThickness;
            paint_rect.position.y = content_rect.position.y + Margin;
            paint_rect.size.width = PaintThickness;
            paint_rect.size.height = content_rect.size.height - Margin * 2;
        }

        canvas.DrawRoundedRectangle(
            zaf::RoundedRect{ paint_rect, 4, 4 },
            ThumbColor());
    }
};

}

ZAF_OBJECT_IMPL(ThinScrollBar);

void ThinScrollBar::Initialize() {

    __super::Initialize();

    this->SetArrowLength(0);
    this->SetThumb(zaf::Create<ThinScrollBarThumb>());
    this->SetSmallChange(20);
    this->SetLargeChange(200);
}

}