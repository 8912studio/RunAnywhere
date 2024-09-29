#pragma once

#include <zaf/control/list_item.h>
#include "module/ai/gpt/dialog/list/dialog_item_data.h"

namespace ra::mod::ai::gpt {

class DialogItemView : public zaf::ListItem {
public:
    ZAF_OBJECT;

    explicit DialogItemView(std::shared_ptr<DialogItemData>);

protected:
    void AfterParse() override;

private:
    std::wstring GenerateSubject() const;

private:
    std::shared_ptr<DialogItemData> dialog_data_;
};

ZAF_OBJECT_BEGIN(DialogItemView);
ZAF_OBJECT_END;

}
