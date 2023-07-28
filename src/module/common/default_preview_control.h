#pragma once

#include "module/command_preview_control.h"
#include "module/common/text_content_control.h"

namespace ra::mod {

class DefaultPreviewControl : public CommandPreviewControl {
public:
    void SetText(const std::wstring& text);

    zaf::Frame GetExpectedMargin() override;

protected:
    void Initialize() override;
    void OnStyleChanged() override;

private:
    std::shared_ptr<TextContentControl> text_control_;
};

}
