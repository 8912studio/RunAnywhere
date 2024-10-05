#pragma once

#include <zaf/control/list_item.h>
#include "module/ai/gpt/dialog/dialog.h"

namespace ra::mod::ai::gpt {

class DialogItemView : public zaf::ListItem {
public:
    ZAF_OBJECT;

    explicit DialogItemView(std::shared_ptr<Dialog>);

protected:
    void AfterParse() override;

private:
    std::wstring GenerateSubject() const;

private:
    std::shared_ptr<Dialog> dialog_;
};

ZAF_OBJECT_BEGIN(DialogItemView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/list/dialog_item_view.xaml");
ZAF_OBJECT_END;

}
