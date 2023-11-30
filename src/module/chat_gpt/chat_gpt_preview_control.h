#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"
#include "module/chat_gpt/answer_result.h"
#include "utility/thin_scroll_control.h"

namespace ra::mod::chat_gpt {

class ChatGPTPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void ShowQuestion(const std::wstring& question);
    void ShowAnswer(zaf::Observable<AnswerResult> observable_answer);

    zaf::Frame GetExpectedMargin() override;

protected:
    void OnStyleChanged() override;

private:
    void ShowContentView(const std::shared_ptr<zaf::Control>& view);

private:
    ZAF_BIND_CONTROL(zaf::Control, contentView);
};

}