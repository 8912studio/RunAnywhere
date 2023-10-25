#pragma once

#include <zaf/control/control.h>

namespace ra::mod::chat_gpt {

class ProgressIndicator : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

    void StartAnimation();
    void StopAnimation();

protected:
    void AfterParse() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;

private:
    std::size_t highlighted_ellipse_index_{};
    zaf::Subscription timer_;
};

}