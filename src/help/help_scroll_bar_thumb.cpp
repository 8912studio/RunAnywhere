#include "help/help_scroll_bar_thumb.h"
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>

namespace ra::help {

ZAF_DEFINE_TYPE(HelpScrollBarThumb)
ZAF_DEFINE_TYPE_END;


void HelpScrollBarThumb::Paint(zaf::Canvas& canvas, const zaf::Rect&) {

    if (!IsEnabled()) {
        return;
    }

    zaf::Canvas::StateGuard state_guard(canvas);

    canvas.SetBrushWithColor(ThumbColor());
    canvas.DrawRoundedRectangle(zaf::RoundedRect{ ContentRect(), 4, 4 });
}

}