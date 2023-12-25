#include "help/help_style_config.h"
#include <zaf/base/range.h>

using namespace ra::utility::markdown::render;

namespace ra::help {
namespace {

void InitializeHeaderConfigs(StyleConfig& style_config) {

    for (auto index : zaf::Range(0, style_config.header_config.size())) {
        style_config.header_config[index].top_spacing = 4;
    }

    style_config.header_config[0].font_size = 20;
    style_config.header_config[1].font_size = 18;
    style_config.header_config[2].font_size = 16;
    style_config.header_config[3].font_size = 14;
    style_config.header_config[4].font_size = 14;
    style_config.header_config[5].font_size = 14;
}


StyleConfig CreateStyleConfig() {

    StyleConfig style_config;
    style_config.basic_config.font = zaf::Font::Default();
    style_config.basic_config.font.size = 14;
    style_config.basic_config.text_color = zaf::Color::Black();

    style_config.inline_code_config.font_family_name = L"Consolas";
    style_config.inline_code_config.text_color = zaf::Color::FromRGB(0xCC4136);
    style_config.inline_code_config.background_color = zaf::Color::FromRGB(0xF5F5F5);

    style_config.code_block_config.code_config = style_config.inline_code_config;

    style_config.list_config.indent = 8;
    style_config.list_config.item_marker_gap = 8;

    style_config.paragraph_config.line_gap = 4;
    style_config.block_gap = 14;
    style_config.bold_font_weight = zaf::FontWeight::Bold;

    InitializeHeaderConfigs(style_config);
    return style_config;
}

}

const StyleConfig& GetHelpStyleConfig() {
    static const StyleConfig config = CreateStyleConfig();
    return config;
}

}