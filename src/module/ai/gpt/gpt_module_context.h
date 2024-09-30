#pragma once

#include "module/ai/gpt/dialog/dialog_service.h"
#include "module/ai/gpt/dialog/unified_dialog_window.h"

namespace ra::mod::ai::gpt {

class GPTModuleContext {
public:
    explicit GPTModuleContext(std::shared_ptr<DialogService> dialog_service) :
        dialog_service_(std::move(dialog_service)) {

    }

    const std::shared_ptr<gpt::DialogService>& DialogService() const {
        return dialog_service_;
    }

    std::shared_ptr<gpt::UnifiedDialogWindow> UnifiedDialogWindow() const {
        return unified_dialog_window_.lock();
    }

    void SetUnifiedDialogWindow(std::shared_ptr<gpt::UnifiedDialogWindow> window) {
        unified_dialog_window_ = std::move(window);
    }

private:
    std::shared_ptr<gpt::DialogService> dialog_service_;
    std::weak_ptr<gpt::UnifiedDialogWindow> unified_dialog_window_;
};

}