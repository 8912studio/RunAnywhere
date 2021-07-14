#include "module/rgb/rgb_preview_control.h"
#include <sstream>
#include <zaf/base/string/case_conversion.h>
#include <zaf/reflection/reflection_type_definition.h>

namespace ra::module::rgb {
namespace {

std::wstring GetColorText(const zaf::Color& color) {

    COLORREF rgb_value = color.ToCOLORREF();

    std::wostringstream stream;
    stream << L"#FF"
        << std::hex
        << std::setw(2) << std::setfill(L'0') << GetRValue(rgb_value)
        << std::setw(2) << std::setfill(L'0') << GetGValue(rgb_value)
        << std::setw(2) << std::setfill(L'0') << GetBValue(rgb_value);

    return zaf::ToUppercased(stream.str());
}

}

ZAF_DEFINE_REFLECTION_TYPE(RGBPreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/rgb/rgb_preview_control.xaml")
ZAF_DEFINE_END


RGBPreviewControl::RGBPreviewControl() {

}


RGBPreviewControl::RGBPreviewControl(const RGBCommandParseResult& parse_result) : 
    parse_result_(parse_result) {

}


void RGBPreviewControl::AfterParse() {

    __super::AfterParse();

    colorControl->SetBackgroundColor(parse_result_.color);
    valueControl->SetText(GetColorText(parse_result_.color));
}


std::wstring RGBPreviewControl::GetText() const {
    return valueControl->GetText();
}

}