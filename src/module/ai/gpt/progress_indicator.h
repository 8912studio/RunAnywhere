#pragma once

#include <zaf/control/control.h>

namespace ra::mod::ai::gpt {

class ProgressIndicator : public zaf::Control {
public:
    ZAF_OBJECT;

    void StartAnimation();
    void StopAnimation();

protected:
    void AfterParse() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) const override;

private:
    std::size_t highlighted_ellipse_index_{};
    zaf::Subscription timer_;
};

ZAF_OBJECT_BEGIN(ProgressIndicator)
ZAF_OBJECT_END;

}