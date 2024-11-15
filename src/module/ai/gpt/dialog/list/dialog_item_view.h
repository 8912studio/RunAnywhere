#pragma once

#include <zaf/control/label_list_item.h>
#include "module/ai/gpt/dialog/dialog.h"

namespace ra::mod::ai::gpt {

class DialogItemView : public zaf::LabelListItem {
public:
    ZAF_OBJECT;

protected:
    void AfterParse() override;
    void OnItemDataChanged() override;

private:
    std::wstring GenerateSubject() const;
};

ZAF_OBJECT_BEGIN(DialogItemView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/list/dialog_item_view.xaml");
ZAF_OBJECT_END;

}
