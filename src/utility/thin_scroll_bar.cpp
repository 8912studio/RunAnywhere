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

        canvas.DrawRoundedRectangle(
            zaf::RoundedRect{ ContentRect(), 4, 4 }, 
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