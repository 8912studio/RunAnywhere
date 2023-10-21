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

    scrollControl->ScrollContent()->AddChild(view);
}


void PreservedCommandListView::RemoveView(const std::shared_ptr<PreservedCommandView>& view) {

    scrollControl->ScrollContent()->RemoveChild(view);
}


void PreservedCommandListView::RemoveFirstView() {

    const auto& scroll_content = scrollControl->ScrollContent();
    if (scroll_content->ChildCount() > 0) {
        scroll_content->RemoveChildAtIndex(0);
    }
}


std::size_t PreservedCommandListView::ViewCount() const {
    return scrollControl->ScrollContent()->ChildCount();
}

}