#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/list_control.h>
#include <zaf/control/list_control_delegate.h>
#include "module/ai/gpt/dialog/list/dialog_data_source.h"

namespace ra::mod::ai::gpt {

class DialogListView : public zaf::Control, public zaf::ListControlDelegate {
public:
    ZAF_OBJECT;

    std::shared_ptr<zaf::Button> NewDialogButton() const;
    std::shared_ptr<zaf::ListControl> ListControl() const;

    void SetDataSource(std::shared_ptr<DialogDataSource> data_source);

    float EstimateItemHeight(
        std::size_t item_index,
        const std::shared_ptr<zaf::Object>& item_data) override;

    std::shared_ptr<zaf::ListItem> CreateItem(
        std::size_t item_index,
        const std::shared_ptr<zaf::Object>& item_data) override;

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(zaf::Button, newDialogButton);
    ZAF_BIND_CONTROL(zaf::ListControl, listControl);
};

ZAF_OBJECT_BEGIN(DialogListView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/list/dialog_list_view.xaml");
ZAF_OBJECT_END;

}