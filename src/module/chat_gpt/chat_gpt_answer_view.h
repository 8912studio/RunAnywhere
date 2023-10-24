#pragma once

#include <zaf/control/linear_box.h>
#include "utility/markdown/render/markdown_region.h"

namespace ra::mod::chat_gpt {

class ChatGPTAnswerView : public zaf::VerticalBox {
public:
    void SetAnswer(zaf::Observable<std::wstring> observable_answer);

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;

private:
    void ResetHeight();

private:
    std::shared_ptr<utility::markdown::render::MarkdownRegion> markdown_region_;
};

}