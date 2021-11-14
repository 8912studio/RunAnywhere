#pragma once

#include <zaf/graphic/color.h>

namespace ra::module::rgb {

class RGBCommandParseResult {
public:
    enum class Format {
        ARGB,
        DecimalIntegerComponents,
        DecimalFloatComponents,
        HexComponents,
    };

public:
    zaf::Color color;
    bool reserve_alpha{};
    Format format{ Format::ARGB };
};

}