#pragma once

#include <zaf/window/window.h>
#include "module/ai/gpt/dialog/unified_dialog_view.h"

namespace ra::mod::ai::gpt {

class UnifiedDialogWindow : public zaf::Window {
public:
    explicit UnifiedDialogWindow(std::shared_ptr<UnifiedDialogModel> model);

    const std::shared_ptr<UnifiedDialogView>& View() const {
        return view_;
    }

protected:
    void AfterParse() override;

    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;

private:
    bool HandleKeyDownMessage(const zaf::KeyMessage& message);

private:
    std::shared_ptr<UnifiedDialogView> view_;
};

}