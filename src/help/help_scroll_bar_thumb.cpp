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

    auto state_guard = canvas.PushState();

    canvas.SetBrushWithColor(ThumbColor());
    canvas.DrawRoundedRectangle(zaf::RoundedRect{ ContentRect(), 4, 4 });
}

}