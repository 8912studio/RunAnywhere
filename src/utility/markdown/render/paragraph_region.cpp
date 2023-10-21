#include "utility/markdown/render/paragraph_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>

namespace ra::utility::markdown::render {
namespace {

//We fail to find a proper way to calculate the default line height,
//so we have to do it by creating a mock text box.
float CalculateDefaultLineHeight(const StyleConfig& style_config) {

    auto text_box = zaf::Create<StyledTextBox>();
    text_box->SetPadding({});
    text_box->SetBorder({});
    text_box->SetFont(style_config.basic_config.font);
    text_box->SetText(L"H");
    return text_box->CalculatePreferredSize().height;
}

}

std::shared_ptr<ParagraphRegion> ParagraphRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Paragraph);

    auto result = zaf::Init(new ParagraphRegion());
    result->InitializeContent(element, style_config);
    result->InitializeStyle(style_config);
    return result;
}


void ParagraphRegion::InitializeStyle(const StyleConfig& style_config) {

    if (style_config.paragraph_config.line_gap == 0) {
        return;
    }

    auto default_line_height = CalculateDefaultLineHeight(style_config);
    auto new_line_height = default_line_height + style_config.paragraph_config.line_gap;

    TextBox()->SetLineSpacing(zaf::LineSpacing{
        zaf::LineSpacingMethod::Uniform, 
        new_line_height,
        new_line_height * 0.8f,
    });

    TextBox()->SetLineGap(style_config.paragraph_config.line_gap);
}

}