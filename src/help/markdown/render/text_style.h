#pragma once

#include <zaf/graphic/color.h>
#include <zaf/graphic/font/font.h>

namespace ra::help::markdown::render {

class TextStyle {
public:
    zaf::Font font;
    zaf::Color text_color;
    zaf::Color background_color;
};

}