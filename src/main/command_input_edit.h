#pragma once

#include <zaf/control/rich_edit.h>
#include "utility/command_line.h"

namespace ra {

class CommandInputEdit : public zaf::RichEdit {
public:
    ZAF_DECLARE_TYPE;

    utility::CommandLine GetInputCommandLine();

protected:
    void Initialize() override;
    void OnTextChanging(const zaf::TextChangingInfo& event_info) override;
};

}