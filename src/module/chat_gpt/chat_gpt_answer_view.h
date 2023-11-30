#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include "module/chat_gpt/answer_result.h"
#include "module/chat_gpt/progress_indicator.h"
#include "module/common/error_view.h"
#include "utility/markdown/render/markdown_region.h"

namespace ra::mod::chat_gpt {

class ChatGPTAnswerView : public zaf::VerticalBox {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetAnswer(zaf::Observable<AnswerResult> observable_answer);

    void ChangeStyle(CommandDisplayStyle style);

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;

private:
    void ShowAnswer(const std::wstring& answer);
    void ShowContent(const std::shared_ptr<zaf::Control>& content);
    void ShowError(const zaf::Error& error);
    void ResetContentHeight();

private:
    ZAF_BIND_CONTROL(ProgressIndicator, progressIndicator);
    ZAF_BIND_CONTROL(zaf::Control, contentView);
    ZAF_BIND_CONTROL(ErrorView, errorView);

    std::shared_ptr<utility::markdown::render::MarkdownRegion> markdown_region_;
};

}