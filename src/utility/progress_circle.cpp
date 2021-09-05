#include "utility/progress_circle.h"
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/matrix.h>
#include <zaf/graphic/resource_factory.h>
#include <zaf/object/type_definition.h>

#undef max
#undef min

namespace ra::utility {
namespace {

zaf::PathGeometry MakeExcludedPath(
	zaf::Canvas& canvas,
	const zaf::Geometry& path,
	const zaf::Geometry& excluded) {

	auto path_geometry = canvas.CreatePathGeometry();
	auto path_sink = path_geometry.Open();

	zaf::Geometry::Combine(
		path,
		excluded,
		zaf::Geometry::CombineMode::Exclude,
		path_sink);

	path_sink.Close();
	return path_geometry;
}

}

ZAF_DEFINE_TYPE(ProgressCircle)
ZAF_DEFINE_TYPE_END


void ProgressCircle::AfterParse() {

	__super::AfterParse();
}


void ProgressCircle::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

	__super::Paint(canvas, dirty_rect);

	auto content_size = ContentSize();
	auto size = Size();

	zaf::Ellipse outer_ellipse;
	outer_ellipse.x_radius = std::min(content_size.width, content_size.height) / 2;
	outer_ellipse.y_radius = outer_ellipse.x_radius;
	outer_ellipse.position.x = size.width / 2;
	outer_ellipse.position.y = size.height / 2;

	zaf::Ellipse inner_ellipse;
	inner_ellipse.x_radius = std::max(outer_ellipse.x_radius - 7, 0.f);
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
	const zaf::Geometry& inner_ellipse_path,
	const zaf::Color& color) {

	auto outer_ellipse_path = canvas.CreateEllipseGeometry(outer_ellipse);
	auto path_geometry = MakeExcludedPath(canvas, outer_ellipse_path, inner_ellipse_path);

	canvas.SetBrushWithColor(color);
	canvas.DrawGeometry(path_geometry);
}


void ProgressCircle::PaintPartialCircle(
	zaf::Canvas& canvas,
	const zaf::Ellipse& outer_ellipse,
	const zaf::Geometry& inner_ellipse_path,
	bool is_for_sink,
	const zaf::Color& color) {

	auto outer_ellipse_path = CreateEllipsePath(
		canvas,
		outer_ellipse,
		is_for_sink);

	auto path_geometry = MakeExcludedPath(canvas, outer_ellipse_path, inner_ellipse_path);

	canvas.SetBrushWithColor(color);
	canvas.DrawGeometry(path_geometry);
}


zaf::Geometry ProgressCircle::CreateEllipsePath(
	zaf::Canvas& canvas,
	const zaf::Ellipse& ellipse,
	bool is_for_sink) {

	auto path_geometry = canvas.CreatePathGeometry();
	auto path_sink = path_geometry.Open();

	path_sink.BeginFigure(ellipse.position, zaf::GeometrySink::BeginFigureOption::Fill);

	zaf::Point begin_point = ellipse.position;
	begin_point.y -= ellipse.y_radius;
	path_sink.AddLine(begin_point);

	auto degress = GetProgressDegrees();

	auto transform_matrix = zaf::TransformMatrix::Rotation(
		degress,
		ellipse.position);

	zaf::ArcSegment arc_segment;
	arc_segment.SetEndPoint(transform_matrix.TransformPoint(begin_point));
	arc_segment.SetXRadius(ellipse.x_radius);
	arc_segment.SetYRadius(ellipse.y_radius);
	arc_segment.SetSweepDirection( 
		is_for_sink ? zaf::SweepDirection::CounterClockwise : zaf::SweepDirection::Clockwise);
	arc_segment.SetArcSize(
		degress < 180 ?
		is_for_sink ? zaf::ArcSize::Large : zaf::ArcSize::Small :
		is_for_sink ? zaf::ArcSize::Small : zaf::ArcSize::Large);

	path_sink.AddArc(arc_segment);

	path_sink.EndFigure(zaf::GeometrySink::EndFigureOption::Close);
	path_sink.Close();

	return path_geometry;
}


float ProgressCircle::GetProgressDegrees() {

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

}