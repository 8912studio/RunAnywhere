#include "module/rgb/rgb_preview_control.h"
#include <sstream>
#include <zaf/base/string/case_conversion.h>
#include <zaf/object/type_definition.h>
#include <zaf/graphic/image/wic/imaging_factory.h>
#include <zaf/graphic/resource_factory.h>

namespace ra::module::rgb {
namespace {

zaf::Color GetStraightColorFromPremultipledColor(const zaf::Color& premultipled_color) {

    return zaf::Color{ 
        premultipled_color.r / premultipled_color.a,
        premultipled_color.g / premultipled_color.a,
        premultipled_color.b / premultipled_color.a,
        premultipled_color.a
    };
}

}


ZAF_DEFINE_TYPE(RGBPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/rgb/rgb_preview_control.xaml")
ZAF_DEFINE_TYPE_END


RGBPreviewControl::RGBPreviewControl() {

}


RGBPreviewControl::RGBPreviewControl(const RGBCommandParseResult& parse_result) : 
    parse_result_(parse_result) {

}


void RGBPreviewControl::AfterParse() {

    __super::AfterParse();

    auto renderer_color = GetRenderedColor();

    colorControl->SetBackgroundColor(renderer_color);
    valueControl->SetText(GetColorText(renderer_color));
}



zaf::Color RGBPreviewControl::GetRenderedColor() {

    zaf::Size bitmap_size{ 1, 1 };

    auto bitmap = zaf::GetImagingFactory().CreateBitmap(bitmap_size);
    auto bitmap_renderer = zaf::GetGraphicFactory().CreateBitmapRenderer(bitmap, {});

    bitmap_renderer.BeginDraw();

    zaf::Rect draw_rect{ zaf::Point{},  bitmap_size };

    //First, draw a white background.
    if (!parse_result_.reserve_alpha) {

        bitmap_renderer.DrawRectangle(
            draw_rect,
            bitmap_renderer.CreateSolidColorBrush(zaf::Color::White()));
    }

    //Then, draw the color.
    bitmap_renderer.DrawRectangle(
        draw_rect,
        bitmap_renderer.CreateSolidColorBrush(parse_result_.color));

    bitmap_renderer.EndDraw();

    //Get value of pixel in bitmap.
    std::uint32_t pixel{};
    bitmap.CopyPixels(draw_rect, 4, 4, reinterpret_cast<std::uint8_t*>(&pixel));

    auto result = zaf::Color::FromARGB(pixel);

    if (parse_result_.reserve_alpha) {
        result = GetStraightColorFromPremultipledColor(result);
    }

    return result;
}


std::wstring RGBPreviewControl::GetColorText(const zaf::Color& color) {

    switch (parse_result_.format) {
    case RGBCommandParseResult::Format::DecimalInteger:
        return GetColorDecimalIntegerText(color);
    case RGBCommandParseResult::Format::DecimalFloat:
        return GetColorDecimalFloatText(color);
    default:
        return GetColorHexText(color);
    }
}


std::wstring RGBPreviewControl::GetColorHexText(const zaf::Color& color) {

    COLORREF rgb_value = color.ToCOLORREF();

    std::wostringstream stream;
    stream << L'#' << std::hex;

    if (parse_result_.reserve_alpha) {
        stream << std::setw(2) << std::setfill(L'0') << static_cast<int>(color.a * 255.f);
    }

    stream 
        << std::setw(2) << std::setfill(L'0') << GetRValue(rgb_value)
        << std::setw(2) << std::setfill(L'0') << GetGValue(rgb_value)
        << std::setw(2) << std::setfill(L'0') << GetBValue(rgb_value);

    return zaf::ToUppercased(stream.str());
}


std::wstring RGBPreviewControl::GetColorDecimalIntegerText(const zaf::Color& color) {

    COLORREF rgb_value = color.ToCOLORREF();

    std::wostringstream stream;
    stream
        << GetRValue(rgb_value) << L','
        << GetGValue(rgb_value) << L','
        << GetBValue(rgb_value);

    return stream.str();
}


std::wstring RGBPreviewControl::GetColorDecimalFloatText(const zaf::Color& color) {

    auto float_to_string = [](float value) {
    
        auto string = std::to_wstring(value);

        auto erase_index = string.find_last_not_of(L'0');
        if (erase_index != std::wstring::npos) {

            if (string[erase_index] == L'.') {
                erase_index += 2;
            }
            else {
                erase_index += 1;
            }

            string.erase(erase_index);
        }

        return string;
    };

    std::wostringstream stream;
    stream
        << float_to_string(color.r) << L','
        << float_to_string(color.g) << L','
        << float_to_string(color.b);

    return stream.str();
}


std::wstring RGBPreviewControl::GetText() const {
    return valueControl->Text();
}

}