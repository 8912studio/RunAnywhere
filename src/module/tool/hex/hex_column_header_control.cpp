#include "module/tool/hex/hex_column_header_control.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>
#include "module/tool/hex/paint_common.h"

namespace ra::module::tool::hex {

ZAF_DEFINE_TYPE(HexColumnHeaderControl)
ZAF_DEFINE_TYPE_END;


void HexColumnHeaderControl::Initialize() {

    __super::Initialize();

    this->SetFixedHeight(LineHeight);
}


void HexColumnHeaderControl::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    canvas.SetBrushWithColor(HeaderBackgroundColor());
    canvas.DrawRectangle(this->ContentRect());

    canvas.SetBrushWithColor(HeaderTextColor());

    for (auto index : zaf::Range(0, BytesPerLine)) {

        auto painted_text = ToHexString(index, 2);
        auto text_layout = CreateCommonTextLayout(painted_text, ByteWidth);

        zaf::Point paint_position;
        paint_position.x = 
            ByteHexAreaX() + 
            index * ByteWidth + 
            (index >= BytesPerLine / 2 ? ByteGapWidth : 0);

        paint_position.y = 0;
        canvas.DrawTextLayout(text_layout, paint_position);
    }

    auto ascii_text_layout = CreateCommonTextLayout(L"ASCII", std::numeric_limits<float>::max());
    ascii_text_layout.SetTextAlignment(zaf::TextAlignment::Left);
    canvas.DrawTextLayout(ascii_text_layout, zaf::Point{ ByteCharacterAreaX(), 0 });
}

}