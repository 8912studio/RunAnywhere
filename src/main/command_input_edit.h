#pragma once

#include <zaf/control/rich_edit.h>

namespace ra {

class CommandInputEdit : public zaf::RichEdit {
public:
    ZAF_DECLARE_TYPE;

protected:
    void Initialize() override;
    void OnTextChanging(const zaf::TextChangingInfo& event_info) override;
};

}