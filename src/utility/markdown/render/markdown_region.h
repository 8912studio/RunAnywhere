#pragma once

#include <memory>
#include <vector>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/body_region.h"

namespace ra::utility::markdown::render {

class MarkdownRegion : public RenderRegion {
public:
    static std::shared_ptr<MarkdownRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

public:
    void ChangeSelection(
        const zaf::Point& current_position,
        const zaf::Point& begin_position) override;

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

    void OnMouseDown(const zaf::PreMouseDownInfo&) override;
    void OnMouseUp(const zaf::PreMouseUpInfo&) override;
    void OnMouseMove(const zaf::PreMouseMoveInfo&) override;

private:
    explicit MarkdownRegion(std::shared_ptr<BodyRegion> body_region);

private:
    std::shared_ptr<BodyRegion> body_region_;

    std::optional<zaf::Point> begin_selection_position_;
};

}