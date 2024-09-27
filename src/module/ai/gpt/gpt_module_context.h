#pragma once

#include "module/ai/gpt/dialog/unified_dialog_window.h"

namespace ra::mod::ai::gpt {

class GPTModuleContext {
public:
    explicit GPTModuleContext(std::shared_ptr<gpt::UnifiedDialogWindow> unified_dialog_window) :
        unified_dialog_window_(std::move(unified_dialog_window)) {

    }

    const std::shared_ptr<gpt::UnifiedDialogWindow>& UnifiedDialogWindow() const {
        return unified_dialog_window_;
    }

private:
    std::shared_ptr<gpt::UnifiedDialogWindow> unified_dialog_window_;
};

}