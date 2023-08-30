#include "utility/thin_scroll_bar.h"
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>

namespace ra::utility {
namespace {

class ThinScrollBarThumb : public zaf::ScrollBarThumb {
protected:
    void Paint(zaf::Canvas& canvas, const zaf::Rect&) override {

        if (!IsEnabled()) {
            return;
        }

        constexpr float PaintThickness = 6;

        auto content_rect = ContentRect();

        zaf::Rect paint_rect;
        if (IsHorizontal()) {
            paint_rect.position.x = content_rect.position.x;
            paint_rect.position.y = 
                content_rect.position.y + content_rect.size.height - PaintThickness;
            paint_rect.size.width = content_rect.size.width;
            paint_rect.size.height = PaintThickness;
        }
        else {
            paint_rect.position.x = 
                content_rect.position.x + content_rect.size.width - PaintThickness;
            paint_rect.position.y = content_rect.position.y;
            paint_rect.size.width = PaintThickness;
            paint_rect.size.height = content_rect.size.height;
        }

        canvas.DrawRoundedRectangle(
            zaf::RoundedRect{ paint_rect, 4, 4 },
            ThumbColor());
    }
};

}

ZAF_DEFINE_TYPE(ThinScrollBar)
ZAF_DEFINE_TYPE_END;


void ThinScrollBar::Initialize() {

    __super::Initialize();

    this->SetArrowLength(0);
    this->SetThumb(zaf::Create<ThinScrollBarThumb>());
    this->SetSmallChange(20);
    this->SetLargeChange(200);
}

}