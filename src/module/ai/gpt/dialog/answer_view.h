#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include "utility/markdown/render/markdown_region.h"

namespace ra::mod::chat_gpt {

class AnswerView : public zaf::VerticalBox {
public:
    ZAF_OBJECT;

public:
    void SetAnswer(zaf::Observable<std::wstring> observable_answer);

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;

private:
    void ShowAnswer(const std::wstring& answer);
    void ShowError(const std::exception_ptr& error);
    static std::shared_ptr<zaf::Control> CreateMajorErrorControl(const std::exception_ptr& error);
    static std::shared_ptr<zaf::Control> CreateDetailErrorControl(const std::exception_ptr& error);
    void ResetContentHeight();

private:
    ZAF_BIND_CONTROL(zaf::Control, contentView);

    std::shared_ptr<utility::markdown::render::MarkdownRegion> markdown_region_;
};

ZAF_OBJECT_BEGIN(AnswerView)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/answer_view.xaml")
ZAF_OBJECT_END;

}