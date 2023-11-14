#pragma once

#include <memory>
#include <vector>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/body_region.h"

namespace ra::utility::markdown::render {

class MarkdownRegion : public zaf::Control {
public:
    static std::shared_ptr<MarkdownRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

    void OnMouseDown(const zaf::MouseDownInfo&) override;
    void OnMouseUp(const zaf::MouseUpInfo&) override;
    void OnMouseMove(const zaf::MouseMoveInfo&) override;

private:
    explicit MarkdownRegion(std::shared_ptr<BodyRegion> body_region);

private:
    std::shared_ptr<BodyRegion> body_region_;

    std::optional<zaf::Point> begin_selection_position_;
};

}