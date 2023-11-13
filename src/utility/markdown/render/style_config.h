#pragma once

#include <array>
#include <string>
#include <zaf/graphic/color.h>
#include <zaf/graphic/font/font.h>
#include "utility/markdown/element/header_element.h"

namespace ra::utility::markdown::render {

class StyleConfig {
public:
    struct BasicConfig {
        zaf::Font font;
        zaf::Color text_color;
    };

    struct ParagraphConfig {
        float line_gap{};
    };

    struct HeaderConfig {
        float font_size{};
        float top_spacing{};
    };

    struct CodeConfig {
        std::wstring font_family_name;
        zaf::Color text_color;
        zaf::Color background_color;
    };

    struct ListConfig {
        float indent{};
        float item_marker_gap{};
    };

public:
    const HeaderConfig& GetHeaderConfig(element::HeaderDepth depth) const {
        auto index = static_cast<int>(depth) - 1;
        return header_config[index];
    }

public:
    BasicConfig basic_config;
    ParagraphConfig paragraph_config;
    std::array<HeaderConfig, 6> header_config;
    CodeConfig inline_code_config;
    CodeConfig code_block_config;
    ListConfig list_config;
    zaf::FontWeight bold_font_weight{};
    float block_gap{};
};

}