#include "module/chat_gpt/chat_gpt_preview_control.h"
#include <zaf/object/type_definition.h>

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(ChatGPTPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/chat_gpt_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void ChatGPTPreviewControl::SetAnswer(const std::wstring& answer) {

    answerControl->SetText(answer);
}

}