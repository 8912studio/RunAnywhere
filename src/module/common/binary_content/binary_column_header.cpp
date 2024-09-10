#include "module/common/binary_content/binary_column_header.h"
#include <zaf/graphic/canvas.h>
#include "module/common/binary_content/binary_content_common.h"

namespace ra::mod {

ZAF_OBJECT_IMPL(BinaryColumnHeader);

void BinaryColumnHeader::Initialize() {

    __super::Initialize();

    this->SetFixedHeight(LineHeight);
}


void BinaryColumnHeader::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) const {

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
    ascii_text_layout.SetTextAlignment(zaf::dwrite::TextAlignment::Left);
    canvas.DrawTextLayout(ascii_text_layout, zaf::Point{ ByteCharacterAreaX(), 0 });
}

}