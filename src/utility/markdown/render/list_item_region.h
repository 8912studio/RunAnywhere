#pragma once

#include <memory>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/body_region.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/styled_text_box.h"

namespace ra::utility::markdown::render {

class ListItemRegion : public RenderRegion {
public:
    static std::shared_ptr<ListItemRegion> Create(
        const std::wstring& item_identity,
        const element::Element& element, 
        const StyleConfig& style_config,
        std::size_t depth);

protected:
    void Initialize() override;
    void Layout(const zaf::Rect&) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    explicit ListItemRegion(std::shared_ptr<BodyRegion> body_region);

    void InitializeStyle(const std::wstring& identity, const StyleConfig& style_config);

private:
    std::shared_ptr<StyledTextBox> identity_text_box_;
    std::shared_ptr<BodyRegion> body_region_;
    float indent_{};
    float identity_gap_{};
};

}