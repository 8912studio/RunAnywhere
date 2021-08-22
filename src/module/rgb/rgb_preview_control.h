#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
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
    RGBCommandParseResult parse_result_;

    ZAF_BIND_CONTROL(zaf::Control, colorControl);
    ZAF_BIND_CONTROL(zaf::Label, valueControl);
};

}