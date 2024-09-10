#include "utility/progress_circle.h"
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/matrix.h>

#undef max
#undef min

namespace ra::utility {
namespace {

zaf::d2d::PathGeometry MakeExcludedPath(
	zaf::Canvas& canvas,
	const zaf::d2d::Geometry& path,
	const zaf::d2d::Geometry& excluded) {

	auto path_geometry = canvas.CreatePathGeometry();
	auto path_sink = path_geometry.Open();

	zaf::d2d::Geometry::Combine(
		path,
		excluded,
		zaf::d2d::Geometry::CombineMode::Exclude,
		path_sink);

	path_sink.Close();
	return path_geometry;
}

}

ZAF_OBJECT_IMPL(ProgressCircle);

void ProgressCircle::AfterParse() {

	__super::AfterParse();
}


void ProgressCircle::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) const {

	__super::Paint(canvas, dirty_rect);

	auto content_size = ContentSize();
	auto size = Size();

	zaf::Ellipse outer_ellipse;
	outer_ellipse.x_radius = std::min(content_size.width, content_size.height) / 2;
	outer_ellipse.y_radius = outer_ellipse.x_radius;
	outer_ellipse.position.y = size.height / 2;

	if (axis_alignment_ == zaf::AxisAlignment::Start) {
		outer_ellipse.position.x = outer_ellipse.x_radius;
	}
	else {
		outer_ellipse.position.x = size.width / 2;
	}

	zaf::Ellipse inner_ellipse;
	inner_ellipse.x_radius = std::max(outer_ellipse.x_radius - thickness_, 0.f);
	inner_ellipse.y_radius = inner_ellipse.x_radius;
	inner_ellipse.position = outer_ellipse.position;

	auto inner_ellipse_path = canvas.CreateEllipseGeometry(inner_ellipse);

	constexpr std::uint32_t SinkColor = 0xe5e5e5;
	constexpr std::uint32_t ProgressColor = 0x52b9ff;

	auto degress = GetProgressDegrees();
	if (degress <= 0) {

		PaintWholeCircle(
			canvas, 
			outer_ellipse,
			inner_ellipse_path,
			zaf::Color::FromRGB(SinkColor));
	}
	else if (degress >= 360) {

		PaintWholeCircle(
			canvas,
			outer_ellipse,
			inner_ellipse_path,
			zaf::Color::FromRGB(ProgressColor));
	}
	else {

		PaintPartialCircle(
			canvas, 
			outer_ellipse, 
			inner_ellipse_path, 
			true, 
			zaf::Color::FromRGB(SinkColor));

		PaintPartialCircle(
			canvas, 
			outer_ellipse, 
			inner_ellipse_path, 
			false, 
			zaf::Color::FromRGB(ProgressColor));
	}
}


void ProgressCircle::PaintWholeCircle(
	zaf::Canvas& canvas,
	const zaf::Ellipse& outer_ellipse,
	const zaf::d2d::Geometry& inner_ellipse_path,
	const zaf::Color& color) const {

	auto outer_ellipse_path = canvas.CreateEllipseGeometry(outer_ellipse);
	auto path_geometry = MakeExcludedPath(canvas, outer_ellipse_path, inner_ellipse_path);

	canvas.SetBrushWithColor(color);
	canvas.DrawGeometry(path_geometry);
}


void ProgressCircle::PaintPartialCircle(
	zaf::Canvas& canvas,
	const zaf::Ellipse& outer_ellipse,
	const zaf::d2d::Geometry& inner_ellipse_path,
	bool is_for_sink,
	const zaf::Color& color) const {

	auto outer_ellipse_path = CreateEllipsePath(
		canvas,
		outer_ellipse,
		is_for_sink);

	auto path_geometry = MakeExcludedPath(canvas, outer_ellipse_path, inner_ellipse_path);

	canvas.SetBrushWithColor(color);
	canvas.DrawGeometry(path_geometry);
}


zaf::d2d::Geometry ProgressCircle::CreateEllipsePath(
	zaf::Canvas& canvas,
	const zaf::Ellipse& ellipse,
	bool is_for_sink) const {

	auto path_geometry = canvas.CreatePathGeometry();
	auto path_sink = path_geometry.Open();

	path_sink.BeginFigure(ellipse.position, zaf::d2d::GeometrySink::BeginFigureOption::Fill);

	zaf::Point begin_point = ellipse.position;
	begin_point.y -= ellipse.y_radius;
	path_sink.AddLine(begin_point);

	auto degress = GetProgressDegrees();

	auto transform_matrix = zaf::TransformMatrix::Rotation(
		degress,
		ellipse.position);

	zaf::d2d::ArcSegment arc_segment;
	arc_segment.SetEndPoint(transform_matrix.TransformPoint(begin_point));
	arc_segment.SetXRadius(ellipse.x_radius);
	arc_segment.SetYRadius(ellipse.y_radius);
	arc_segment.SetSweepDirection( 
		is_for_sink ? 
		zaf::d2d::SweepDirection::CounterClockwise : 
		zaf::d2d::SweepDirection::Clockwise);
	arc_segment.SetArcSize(
		degress < 180 ?
		is_for_sink ? zaf::d2d::ArcSize::Large : zaf::d2d::ArcSize::Small :
		is_for_sink ? zaf::d2d::ArcSize::Small : zaf::d2d::ArcSize::Large);

	path_sink.AddArc(arc_segment);

	path_sink.EndFigure(zaf::d2d::GeometrySink::EndFigureOption::Close);
	path_sink.Close();

	return path_geometry;
}


float ProgressCircle::GetProgressDegrees() const {

	if (max_value_ <= 0) {
		return 0;
	}

	return static_cast<float>(360.f * (double(value_) / double(max_value_)));
}


void ProgressCircle::SetValue(std::uint64_t value) {

	value_ = value;
	NeedRepaint();
}


void ProgressCircle::SetMaxValue(std::uint64_t value) {

	max_value_ = value;
	NeedRepaint();
}


void ProgressCircle::SetAxisAlignment(zaf::AxisAlignment alignment) {

	//End alignment is not supported yet.
	ZAF_EXPECT(alignment != zaf::AxisAlignment::End);

	axis_alignment_ = alignment;
	NeedRepaint();
}


void ProgressCircle::SetThickness(float thickness) {

	thickness_ = thickness;
	NeedRepaint();
}

}