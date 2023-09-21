#pragma once

#include <string>
#include <zaf/graphic/font/font.h>

namespace ra::help::markdown::render {

class StyleConfig {
public:
    struct BasicConfig {
        zaf::Font font;
    } basic_config;

    zaf::FontWeight bold_font_weight{};

    struct HeaderConfig {
        float _1_font_size{};
        float _2_font_size{};
        float _3_font_size{};
        float _4_font_size{};
        float _5_font_size{};
        float _6_font_size{};
    } header_config;

    struct CodeConfig {
        std::wstring font_family_name;
    } code_config;
};

}