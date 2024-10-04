#include "module/ai/gpt/dialog/unified_dialog_window.h"
#include <zaf/creation.h>

namespace ra::mod::ai::gpt {

UnifiedDialogWindow::UnifiedDialogWindow(std::shared_ptr<UnifiedDialogModel> model) {
    view_ = zaf::Create<UnifiedDialogView>(model);
}


void UnifiedDialogWindow::AfterParse() {

    __super::AfterParse();

    this->SetContentSize(zaf::Size{ 800, 600 });
    this->SetMinSize(zaf::Size{ 400, 300 });

    this->SetRootControl(view_);
}

}