#pragma once

#include <zaf/control/control_binder.h>
#include "module/command_preview_control.h"

namespace ra::mod::ai::gpt {

class ChatGPTPreviewControl : public CommandPreviewControl {
public:
    ZAF_OBJECT;

    void ShowQuestion(const std::wstring& question);

protected:
    void OnStyleChanged() override;

private:
    void ShowContentView(const std::shared_ptr<zaf::Control>& view);

private:
    ZAF_BIND_CONTROL(zaf::Control, contentView);
};

ZAF_OBJECT_BEGIN(ChatGPTPreviewControl)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/chat_gpt_preview_control.xaml")
ZAF_OBJECT_END;

}