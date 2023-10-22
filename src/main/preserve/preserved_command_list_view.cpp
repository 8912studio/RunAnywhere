#include "main/preserve/preserved_command_list_view.h"
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(PreservedCommandListView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/preserve/preserved_command_list_view.xaml")
ZAF_DEFINE_TYPE_END;

void PreservedCommandListView::AfterParse() {

    __super::AfterParse();
}


void PreservedCommandListView::AddView(const std::shared_ptr<PreservedCommandView>& view) {

    listView->AddChild(view);
    ResetHeight();
}


void PreservedCommandListView::RemoveView(const std::shared_ptr<PreservedCommandView>& view) {

    listView->RemoveChild(view);
    ResetHeight();
}


void PreservedCommandListView::RemoveFirstView() {

    if (listView->ChildCount() > 0) {
        listView->RemoveChildAtIndex(0);
    }
    ResetHeight();
}


std::size_t PreservedCommandListView::ViewCount() const {
    return listView->ChildCount();
}


void PreservedCommandListView::ResetHeight() {

    float total_height{};

    for (const auto& each_view : listView->Children()) {
        total_height += each_view->Height();
    }

    constexpr float max_height = 500;
    total_height = std::min(total_height, max_height);

    this->SetFixedHeight(total_height);
}

}