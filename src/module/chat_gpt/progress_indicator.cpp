#include "module/chat_gpt/progress_indicator.h"
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/timer.h>

namespace ra::mod::chat_gpt {
namespace {

constexpr int EllipseCount = 3;

}

ZAF_DEFINE_TYPE(ProgressIndicator)
ZAF_DEFINE_TYPE_END;


void ProgressIndicator::StartAnimation() {

    timer_ = zaf::rx::Interval(std::chrono::milliseconds(800), zaf::Scheduler::Main()).Subscribe(
        [this](int) {

        highlighted_ellipse_index_ = (highlighted_ellipse_index_ + 1) % EllipseCount;
        this->NeedRepaint();
    });
}


void ProgressIndicator::StopAnimation() {
    timer_.Unsubscribe();
}


void ProgressIndicator::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    constexpr float ellipse_radius = 4;
    constexpr float ellipse_diameter = ellipse_radius * 2;
    constexpr float ellipse_spacing = ellipse_diameter * 2;

    constexpr float total_width = 
        ellipse_diameter * EllipseCount + 
        ellipse_spacing * (EllipseCount - 1);

    constexpr float total_height = ellipse_diameter;

    float ellipse_x = (this->Width() - total_width) / 2 + ellipse_radius;
    const float ellipse_y = this->Height() / 2;

    for (auto index : zaf::Range(0, EllipseCount)) {

        zaf::Ellipse ellipse{ ellipse_x, ellipse_y, ellipse_radius, ellipse_radius };

        canvas.DrawEllipse(ellipse, canvas.Renderer().CreateSolidColorBrush([this, index]() {

            auto result = zaf::Color::Gray();
            if (index == highlighted_ellipse_index_) {
                result.a *= 0.6f;
            }
            return result;
        }()));

        ellipse_x += ellipse_diameter + ellipse_spacing;
    }
}

}