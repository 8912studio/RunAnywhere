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

public:
    void SetCanSelect(bool can_select);

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

    void OnMouseDown(const zaf::MouseDownInfo&) override;
    void OnMouseUp(const zaf::MouseUpInfo&) override;
    void OnPreMouseMove(const zaf::PreMouseMoveInfo&) override;

    void OnKeyDown(const zaf::KeyDownInfo&) override;

    void OnFocusGained(const zaf::FocusGainedInfo&) override;
    void OnFocusLost(const zaf::FocusLostInfo&) override;

private:
    explicit MarkdownRegion(std::shared_ptr<BodyRegion> body_region);

private:
    std::shared_ptr<BodyRegion> body_region_;
    bool can_select_{};
    std::optional<zaf::Point> begin_selection_position_;
};

}