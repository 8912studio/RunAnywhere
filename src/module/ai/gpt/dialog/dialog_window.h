#pragma once

#include <zaf/window/window.h>
#include "module/ai/gpt/dialog/dialog_view.h"

namespace ra::mod::ai::gpt {

class DialogWindow : public zaf::Window {
public:
    ZAF_OBJECT;

public:
    explicit DialogWindow(std::shared_ptr<Dialog> dialog);

    const std::shared_ptr<DialogView>& View() const {
        return view_;
    }

    void Open();

protected:
    void AfterParse() override;
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;

private:
    bool HandleKeyDownMessage(const zaf::KeyMessage& message);
    void OpenWindow();

private:
    std::shared_ptr<DialogView> view_;
};

ZAF_OBJECT_BEGIN(DialogWindow)
ZAF_OBJECT_END;

}