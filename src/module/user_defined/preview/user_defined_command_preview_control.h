#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/user_defined/command_line_parsing.h"

namespace ra::module::user_defined {

class UserDefinedCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_REFLECTION_TYPE;

    void SetParseResult(const ParseResult& parse_result);

protected:
    void AfterParsing() override;

private:
    void BuildArgumentLabels(const std::vector<std::wstring>& arguments);
    void ResetFixedHeight();

private:
    ZAF_BIND_CONTROL(zaf::Label, commandLabel);
    ZAF_BIND_CONTROL(zaf::Control, argumentContainer);
};

}