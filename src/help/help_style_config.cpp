#include "help/help_style_config.h"

using namespace ra::help::markdown::render;

namespace ra::help {
namespace {

StyleConfig CreateStyleConfig() {

    StyleConfig style_config;
    style_config.basic_config.font = zaf::Font::Default();
    style_config.basic_config.font.size = 14;
    style_config.basic_config.text_color = zaf::Color::Black();

    style_config.inline_code_config.font_family_name = L"Consolas";
    style_config.inline_code_config.text_color = zaf::Color::FromRGB(0xCC4136);
    style_config.inline_code_config.background_color = zaf::Color::FromRGB(0xF5F5F5);

    style_config.code_block_config = style_config.inline_code_config;

    style_config.header_config[0].font_size = 20;
    style_config.header_config[0].top_spacing = 4;
    style_config.header_config[1].font_size = 18;
    style_config.header_config[1].top_spacing = 4;
    style_config.header_config[2].font_size = 16;
    style_config.header_config[2].top_spacing = 4;

    style_config.paragraph_config.line_gap = 4;
    style_config.block_gap = 14;
    style_config.bold_font_weight = zaf::FontWeight::Bold;

    return style_config;
}

}

const StyleConfig& GetHelpStyleConfig() {
    static const StyleConfig config = CreateStyleConfig();
    return config;
}

}