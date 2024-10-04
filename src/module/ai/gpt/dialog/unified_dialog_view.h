#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/split_control.h>
#include "module/ai/gpt/dialog/content/dialog_view.h"
#include "module/ai/gpt/dialog/list/dialog_list_view.h"
#include "module/ai/gpt/dialog/unified_dialog_model.h"

namespace ra::mod::ai::gpt {

class UnifiedDialogView : public zaf::Control {
public:
    ZAF_OBJECT;

public:
    explicit UnifiedDialogView(std::shared_ptr<UnifiedDialogModel> model);

    void StartNewDialog(std::wstring question);

protected:
    void AfterParse() override;

private:
    void OpenDialogView(std::shared_ptr<Dialog> dialog);

    void OnListSelectionChanged(const zaf::ListControlSelectionChangedInfo& event_info);

private:
    ZAF_BIND_CONTROL(zaf::SplitControl, splitControl);
    ZAF_BIND_CONTROL(DialogListView, listView);

    std::shared_ptr<UnifiedDialogModel> model_;
    std::shared_ptr<DialogView> current_dialog_view_;
};

ZAF_OBJECT_BEGIN(UnifiedDialogView)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/unified_dialog_view.xaml")
ZAF_OBJECT_END;

}