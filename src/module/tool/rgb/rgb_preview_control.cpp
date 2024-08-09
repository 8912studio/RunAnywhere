#include "module/tool/rgb/rgb_preview_control.h"
#include <sstream>
#include <zaf/base/string/case_conversion.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/image/wic/imaging_factory.h>
#include "module/common/style_constants.h"

namespace ra::mod::tool::rgb {
namespace {

template<bool IsHistorical> 
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
    static constexpr float FixedHeight = 90;
    static constexpr auto AxisAlignment = zaf::AxisAlignment::Center;
    static constexpr float FontSize = 26;
    static constexpr float TextLeftMargin = 15;

    static zaf::Size ColorControlSize() {
        return zaf::Size{ 96, 48 };
    }
};

template<>
struct StyleMetrics<true> {
    static constexpr float FixedHeight = 28;
    static constexpr auto AxisAlignment = zaf::AxisAlignment::Start;
    static constexpr float FontSize = StyleConstants::PreservedBodyFontSize;
    static constexpr float TextLeftMargin = 8;

    static zaf::Size ColorControlSize() {
        return zaf::Size{ 38, 20 };
    }
};


zaf::Color GetStraightColorFromPremultipledColor(const zaf::Color& premultipled_color) {

    return zaf::Color{ 
        premultipled_color.r / premultipled_color.a,
        premultipled_color.g / premultipled_color.a,
        premultipled_color.b / premultipled_color.a,
        premultipled_color.a
    };
}

}

ZAF_OBJECT_IMPL(RGBPreviewControl);

RGBPreviewControl::RGBPreviewControl(const RGBCommandParseResult& parse_result) : 
    parse_result_(parse_result) {

}


void RGBPreviewControl::AfterParse() {

    __super::AfterParse();

    auto renderer_color = GetRenderedColor();

    colorControl->SetBackgroundColor(renderer_color);
    valueControl->SetText(GetColorText(renderer_color));
}


void RGBPreviewControl::OnStyleChanged() {

    auto set_style = [this](auto metrics) {
        auto update_guard = this->BeginUpdate();
        container->SetAxisAlignment(metrics.AxisAlignment);
        colorControl->SetFixedSize(metrics.ColorControlSize());
        valueControl->SetFontSize(metrics.FontSize);
        valueControl->SetMargin(zaf::Frame{ metrics.TextLeftMargin, 0, 0, 0 });
        this->SetFixedHeight(metrics.FixedHeight);
    };

    if (Style() == CommandDisplayStyle::Preserved) {
        set_style(StyleMetrics<true>{});
    }
    else {
        set_style(StyleMetrics<false>{});
    }
}


zaf::Color RGBPreviewControl::GetRenderedColor() {

    zaf::Size bitmap_size{ 1, 1 };

    auto bitmap = zaf::wic::ImagingFactory::Instance().CreateBitmap(bitmap_size);
    auto bitmap_renderer = zaf::GraphicFactory::Instance().CreateBitmapRenderer(bitmap, {});

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
    case RGBCommandParseResult::Format::DecimalIntegerComponents:
    case RGBCommandParseResult::Format::DecimalFloatComponents:
    case RGBCommandParseResult::Format::HexComponents:
        return GetColorTextInComponentsFormat(color);
    default:
        return GetColorTextInARGBFormat(color);
    }
}


std::wstring RGBPreviewControl::GetColorTextInARGBFormat(const zaf::Color& color) {

    std::wostringstream stream;
    stream << L'#' << std::hex;

    if (parse_result_.reserve_alpha) {
        stream << std::setw(2) << std::setfill(L'0') << int(color.a * 255.f);
    }

    stream 
        << std::setw(2) << std::setfill(L'0') << int(color.r * 255.f)
        << std::setw(2) << std::setfill(L'0') << int(color.g * 255.f)
        << std::setw(2) << std::setfill(L'0') << int(color.b * 255.f);

    return zaf::ToUppercased(stream.str());
}


std::wstring RGBPreviewControl::GetColorTextInComponentsFormat(const zaf::Color& color) {

    std::vector<std::wstring> components;

    switch (parse_result_.format) {
    case RGBCommandParseResult::Format::DecimalIntegerComponents:
        components = GetComponentTextInDecimalInteger(color);
        break;
    case RGBCommandParseResult::Format::DecimalFloatComponents:
        components = GetComponentTextInDecimalFloat(color);
        break;
    case RGBCommandParseResult::Format::HexComponents:
        components = GetComponentTextInHex(color);
        break;
    default:
        break;
    }

    std::wstring result;
    for (auto index : zaf::Range(0u, components.size())) {

        if (index != 0) {
            result += L" , ";
        }

        result += components[index];
    }
    return result;
}


std::vector<std::wstring> RGBPreviewControl::GetComponentTextInDecimalInteger(
    const zaf::Color& color) {

    std::vector<std::wstring> result{
        std::to_wstring(int(color.r * 255.f)),
        std::to_wstring(int(color.g * 255.f)),
        std::to_wstring(int(color.b * 255.f)),
    };

    if (parse_result_.reserve_alpha) {
        result.push_back(std::to_wstring(int(color.a * 255.f)));
    }

    return result;
}


std::vector<std::wstring> RGBPreviewControl::GetComponentTextInDecimalFloat(
    const zaf::Color& color) {

    auto float_to_string = [](float value) {
    
        std::wostringstream stream;
        stream.precision(5);
        stream << value;

        auto string = stream.str();

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

    std::vector<std::wstring> result{
        float_to_string(color.r),
        float_to_string(color.g),
        float_to_string(color.b),
    };

    if (parse_result_.reserve_alpha) {
        result.push_back(float_to_string(color.a));
    }

    return result;
}


std::vector<std::wstring> RGBPreviewControl::GetComponentTextInHex(const zaf::Color& color) {

    auto to_hex_string = [](float value) {
    
        std::wostringstream stream;
        stream << std::hex << int(value * 255.f);
        return L"0x" + zaf::ToUppercased(stream.str());
    };

    std::vector<std::wstring> result{
        to_hex_string(color.r),
        to_hex_string(color.g),
        to_hex_string(color.b),
    };

    if (parse_result_.reserve_alpha) {
        result.push_back(to_hex_string(color.a));
    }

    return result;
}


std::wstring RGBPreviewControl::GetText() const {
    return valueControl->Text();
}

}