#pragma once

#include <array>
#include <string>
#include <zaf/graphic/color.h>
#include <zaf/graphic/font/font.h>

namespace ra::help::markdown::render {

class StyleConfig {
public:
    struct BasicConfig {
        zaf::Font font;
        zaf::Color text_color;
    };

    struct HeaderConfig {
        std::array<float, 6> font_size{};
    };

    struct CodeConfig {
        std::wstring font_family_name;
        zaf::Color text_color;
    };

public:
    BasicConfig basic_config;
    HeaderConfig header_config;
    CodeConfig inline_code_config;
    CodeConfig code_block_config;
    zaf::FontWeight bold_font_weight{};
};

}