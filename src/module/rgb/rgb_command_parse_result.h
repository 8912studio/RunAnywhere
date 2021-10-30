#pragma once

#include <zaf/graphic/color.h>

namespace ra::module::rgb {

class RGBCommandParseResult {
public:
    enum class Format {
        Hex,
        DecimalInteger,
        DecimalFloat,
    };

public:
    zaf::Color color;
    bool has_alpha_part{};
    bool reserve_alpha{};
    Format format{ Format::Hex };
};

}