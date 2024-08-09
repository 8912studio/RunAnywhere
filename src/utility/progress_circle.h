#pragma once

#include <zaf/control/control.h>
#include <zaf/control/layout/axis_alignment.h>
#include <zaf/graphic/geometry/path_geometry.h>

namespace ra::utility {

class ProgressCircle : public zaf::Control {
public:
	ZAF_OBJECT;

	void SetValue(std::uint64_t value);
	void SetMaxValue(std::uint64_t value);

	void SetAxisAlignment(zaf::AxisAlignment alignment);
	void SetThickness(float thickness);

protected:
	void AfterParse() override;
	void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) const override;

private:
	void PaintWholeCircle(
		zaf::Canvas& canvas,
		const zaf::Ellipse& outer_ellipse,
		const zaf::Geometry& inner_ellipse_path, 
		const zaf::Color& color) const;

	void PaintPartialCircle(
		zaf::Canvas& canvas,
		const zaf::Ellipse& outer_ellipse,
		const zaf::Geometry& inner_ellipse_path,
		bool is_for_sink,
		const zaf::Color& color) const;

	zaf::Geometry CreateEllipsePath(
		zaf::Canvas& canvas, 
		const zaf::Ellipse& ellipse,
		bool is_for_sink) const;

	float GetProgressDegrees() const;

private:
	std::uint64_t value_{};
	std::uint64_t max_value_{};

	zaf::AxisAlignment axis_alignment_{ zaf::AxisAlignment::Center };
	float thickness_{ 7 };
};

ZAF_OBJECT_BEGIN(ProgressCircle)
ZAF_OBJECT_END

}