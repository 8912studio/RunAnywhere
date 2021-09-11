#pragma once

#include <zaf/graphic/color.h>
#include <zaf/graphic/text/text_layout.h>

namespace ra::help {

class CellHorizontalLayoutInfo {
public:
    float x{};
    zaf::TextLayout text_layout;
    zaf::Color color{ zaf::Color::Black() };
};


class CellVerticalLayoutInfo {
public:
    float y{};
    CellHorizontalLayoutInfo horizontal_info;
};

}