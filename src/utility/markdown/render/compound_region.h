#pragma once

#include "utility/markdown/element/element.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"

namespace ra::utility::markdown::render {

class CompoundRegion : public RenderRegion {
public:
    bool IsPositionInsideText(const zaf::Point& mouse_position) override;

    void BeginSelection(const zaf::Point& position) override;
    void ChangeSelection(const PositionRange& position_range) override;
    void EndSelection() override;

    void SelectWord(const zaf::Point& position) override;

    void BuildSelectedText(SelectedTextBuilder& builder) override;

    void ChangeFocus(bool is_focused) override;

protected:
    explicit CompoundRegion(std::vector<std::shared_ptr<RenderRegion>> child_regions);

    void Initialize() override;
    void Layout(const zaf::Rect&) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    std::vector<std::shared_ptr<RenderRegion>> child_regions_;
};

}