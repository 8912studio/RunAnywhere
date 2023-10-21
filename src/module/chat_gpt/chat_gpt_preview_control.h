#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"

namespace ra::mod::chat_gpt {

class ChatGPTPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetAnswer(const std::wstring& answer);

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;

private:
    void ResetHeight();

private:
    ZAF_BIND_CONTROL(zaf::TextBox, answerControl);
};

}