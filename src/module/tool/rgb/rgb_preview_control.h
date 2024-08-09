#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include <zaf/graphic/image/wic/bitmap.h>
#include <zaf/graphic/renderer/renderer.h>
#include "module/command_preview_control.h"
#include "module/tool/rgb/rgb_command_parse_result.h"

namespace ra::mod::tool::rgb {

class RGBPreviewControl : public CommandPreviewControl {
public:
    ZAF_OBJECT

    explicit RGBPreviewControl(const RGBCommandParseResult& parse_result);

    std::wstring GetText() const;

protected:
    void AfterParse() override;
    void OnStyleChanged() override;

private:
    zaf::Color GetRenderedColor();
    std::wstring GetColorText(const zaf::Color& color);
    std::wstring GetColorTextInARGBFormat(const zaf::Color& color);
    std::wstring GetColorTextInComponentsFormat(const zaf::Color& color);
    std::vector<std::wstring> GetComponentTextInDecimalInteger(const zaf::Color& color);
    std::vector<std::wstring> GetComponentTextInDecimalFloat(const zaf::Color& color);
    std::vector<std::wstring> GetComponentTextInHex(const zaf::Color& color);

private:
    ZAF_BIND_CONTROL(zaf::HorizontalBox, container);
    ZAF_BIND_CONTROL(zaf::Control, colorControl);
    ZAF_BIND_CONTROL(zaf::Label, valueControl);

    RGBCommandParseResult parse_result_;
};

ZAF_OBJECT_BEGIN(RGBPreviewControl)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/tool/rgb/rgb_preview_control.xaml")
ZAF_OBJECT_END

}