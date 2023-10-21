#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"
#include "utility/markdown/render/markdown_region.h"
#include "utility/thin_scroll_control.h"

namespace ra::mod::chat_gpt {

class ChatGPTPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetAnswer(const std::wstring& answer);

    zaf::Frame GetExpectedMargin() override;

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;

private:
    void ResetHeight();

private:
    std::shared_ptr<utility::markdown::render::MarkdownRegion> markdown_region_;
};

}