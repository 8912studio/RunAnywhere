#pragma once

#include <optional>
#include <zaf/graphic/color.h>
#include <zaf/graphic/font/font.h>

namespace ra::utility::markdown::render {

class TextStyle {
public:
    zaf::Font font;
    zaf::Color text_color;
    std::optional<zaf::Color> background_color;
};

}