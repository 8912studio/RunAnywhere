#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"

namespace ra::mod::chat_gpt {

class ChatGPTPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetQuestion(const std::wstring& question);
    void SetAnswer(const std::wstring& answer);

private:
    ZAF_BIND_CONTROL(zaf::TextBox, questionControl);
    ZAF_BIND_CONTROL(zaf::TextBox, answerControl);
};

}