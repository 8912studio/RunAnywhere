#pragma once

#include <zaf/window/window.h>
#include "module/ai/gpt/dialog/unified_dialog_view.h"

namespace ra::mod::ai::gpt {

class UnifiedDialogWindow : public zaf::Window {
public:
    explicit UnifiedDialogWindow(std::shared_ptr<DialogManager> dialog_manager);

    const std::shared_ptr<UnifiedDialogView>& View() const {
        return view_;
    }

protected:
    void AfterParse() override;

private:
    std::shared_ptr<UnifiedDialogView> view_;
};

}