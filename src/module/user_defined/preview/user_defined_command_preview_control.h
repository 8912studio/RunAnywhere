#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/user_defined/entry.h"
#include "module/user_defined/execute_info.h"

namespace ra::mod::user_defined {

class UserDefinedCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetCommandEntry(const std::shared_ptr<Entry>& entry);
    void SetExecutInfo(ExecuteInfo execute_info);

    const std::shared_ptr<Entry>& GetCommandEntry() const {
        return entry_;
    }

    const ExecuteInfo& GetExecuteInfo() const {
        return execute_info_;
    }

protected:
    void AfterParse() override;
    void OnStyleChanged() override;

private:
    void BuildArgumentLabels(const std::vector<std::wstring>& arguments);
    void AdjustCommandContainerLayout();

private:
    ZAF_BIND_CONTROL(zaf::Label, entryInfoLabel);
    ZAF_BIND_CONTROL(zaf::Control, commandContainer);
    ZAF_BIND_CONTROL(zaf::Label, commandLabel);
    ZAF_BIND_CONTROL(zaf::Control, argumentContainer);
    ZAF_BIND_CONTROL(zaf::Label, workingDirectoryLabel);

    std::shared_ptr<Entry> entry_;
    ExecuteInfo execute_info_;
};

}