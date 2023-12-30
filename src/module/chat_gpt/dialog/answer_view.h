#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include "utility/markdown/render/markdown_region.h"

namespace ra::mod::chat_gpt {

class AnswerView : public zaf::VerticalBox {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetAnswer(zaf::Observable<std::wstring> observable_answer);

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;

private:
    void ShowAnswer(const std::wstring& answer);
    void ShowError(const zaf::Error& error);
    void ShowContent(const std::shared_ptr<zaf::Control>& content);
    void ResetContentHeight();

private:
    ZAF_BIND_CONTROL(zaf::Control, contentView);

    std::shared_ptr<utility::markdown::render::MarkdownRegion> markdown_region_;
};

}