#include "module/ai/gpt/dialog/list/dialog_list_view.h"
#include <zaf/base/as.h>
#include <zaf/creation.h>
#include "module/ai/gpt/dialog/list/dialog_item_view.h"

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogListView);

void DialogListView::AfterParse() {

    __super::AfterParse();

    listControl->SetDelegate(zaf::As<zaf::ListControlDelegate>(shared_from_this()));
}


std::shared_ptr<zaf::ListControl> DialogListView::ListControl() const {
    return listControl;
}


void DialogListView::SetDataSource(std::shared_ptr<DialogDataSource> data_source) {
    listControl->SetDataSource(data_source);
}


float DialogListView::EstimateItemHeight(
    std::size_t item_index,
    const std::shared_ptr<zaf::Object>& item_data) {

    return 40.0f;
}


std::shared_ptr<zaf::ListItem> DialogListView::CreateItem(
    std::size_t item_index, 
    const std::shared_ptr<zaf::Object>& item_data) {

    return zaf::Create<DialogItemView>(zaf::As<DialogItemData>(item_data));
}


void DialogListView::LoadItem(
    const std::shared_ptr<zaf::ListItem>& item, 
    std::size_t item_index) {


}

}
