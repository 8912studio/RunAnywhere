#pragma once

#include <zaf/control/text_box.h>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/text_style.h"

namespace ra::help::markdown::render {

class CodeBlockRegion : public RenderRegion {
public:
    static std::shared_ptr<CodeBlockRegion> Create(
        const element::Element& element, 
        const StyleConfig& style_config);

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    CodeBlockRegion() = default;

    void SetStyledText(const std::wstring& text, const TextStyle& text_style);
    void SetTextBackgroundColor(const zaf::Color& color);

private:
    std::shared_ptr<zaf::TextBox> text_box_;
};

}