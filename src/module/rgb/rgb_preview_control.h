#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/graphic/image/wic/bitmap.h>
#include <zaf/graphic/renderer/renderer.h>
#include "module/command_preview_control.h"
#include "module/rgb/rgb_command_parse_result.h"

namespace ra::module::rgb {

class RGBPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE

    RGBPreviewControl();
    RGBPreviewControl(const RGBCommandParseResult& parse_result);

    std::wstring GetText() const;

protected:
    void AfterParse() override;

private:
    zaf::Color GetRenderedColor();
    std::wstring GetColorText(const zaf::Color& color);
    std::wstring GetColorHexText(const zaf::Color& color);
    std::wstring GetColorDecimalIntegerText(const zaf::Color& color);
    std::wstring GetColorDecimalFloatText(const zaf::Color& color);

private:
    ZAF_BIND_CONTROL(zaf::Control, colorControl);
    ZAF_BIND_CONTROL(zaf::Label, valueControl);

    RGBCommandParseResult parse_result_;
};

}