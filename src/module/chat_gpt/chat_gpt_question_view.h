#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include "main/command_display_style.h"
#include "module/common/error_view.h"
#include "module/common/text_content_control.h"
#include "utility/link_button.h"

namespace ra::mod::chat_gpt {

class ChatGPTQuestionView : public zaf::VerticalBox {
public:
    ZAF_DECLARE_TYPE;

    void SetQuestion(const std::wstring& question);

    void ChangeStyle(CommandDisplayStyle style);

protected:
    void AfterParse() override;

private:
    void ShowNoAPIKeyError();
    void ShowQuestion(const std::wstring& question);

private:
    ZAF_BIND_CONTROL(zaf::Control, hintContainer);
    ZAF_BIND_CONTROL(zaf::Control, messageLabel);
    ZAF_BIND_CONTROL(TextContentControl, questionControl);
    ZAF_BIND_CONTROL(zaf::Control, errorContainer);
    ZAF_BIND_CONTROL(ErrorView, errorView);
    ZAF_BIND_CONTROL(utility::LinkButton, openOptionButton);
};

}