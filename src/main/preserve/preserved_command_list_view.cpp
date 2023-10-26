#include "main/preserve/preserved_command_list_view.h"
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "option/option_storage.h"

namespace ra {

ZAF_DEFINE_TYPE(PreservedCommandListView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/preserve/preserved_command_list_view.xaml")
ZAF_DEFINE_TYPE_END;

void PreservedCommandListView::AfterParse() {

    __super::AfterParse();
}


void PreservedCommandListView::AddView(const std::shared_ptr<PreservedCommandView>& view) {

    {
        auto update_guard = this->BeginUpdate();
        RemoveExcessViews();

        auto view_item = std::make_unique<ViewItem>();
        view_item->view = view;
        view_item->subscriptions += view->StateUpdatedEvent().Subscribe(std::bind(
            &PreservedCommandListView::OnViewStateUpdated,
            this,
            std::placeholders::_1));

        view_items_.push_back(std::move(view_item));
        listView->AddChild(view);
    }

    ResetHeight();
    ScrollToView(view);
}


void PreservedCommandListView::RemoveExcessViews() {

    auto max_count = option::OptionStorage::Instance().MaxPreservedCommandCount();
    while (view_items_.size() >= max_count) {
        RemoveFirstViewWithoutResetHeight();
    }
}


void PreservedCommandListView::OnViewStateUpdated(
    const std::shared_ptr<PreservedCommandView>& view) {

    ResetHeight();

    auto iterator = std::find_if(
        view_items_.begin(),
        view_items_.end(),
        [&view](const auto& item) {
        return item->view == view;
    });

    if (iterator == view_items_.end()) {
        return;
    }

    if (iterator == view_items_.rbegin().base()) {
        ScrollToView((*iterator)->view);
    }
}


void PreservedCommandListView::ScrollToView(const std::shared_ptr<PreservedCommandView>& view) {

    scrollControl->VerticalScrollBar()->SetValue(static_cast<int>(view->Position().y));
}


void PreservedCommandListView::RemoveView(const std::shared_ptr<PreservedCommandView>& view) {

    auto iterator = std::find_if(
        view_items_.begin(),
        view_items_.end(), 
        [&view](const auto& item) {
    
        return item->view == view;
    });

    if (iterator == view_items_.end()) {
        return;
    }

    auto index = std::distance(view_items_.begin(), iterator);
    listView->RemoveChildAtIndex(index);

    view_items_.erase(iterator);

    ResetHeight();
}


void PreservedCommandListView::RemoveFirstView() {

    RemoveFirstViewWithoutResetHeight();
    ResetHeight();
}


void PreservedCommandListView::RemoveFirstViewWithoutResetHeight() {

    if (!view_items_.empty()) {
        listView->RemoveChildAtIndex(0);
        view_items_.erase(view_items_.begin());
    }
}


std::size_t PreservedCommandListView::ViewCount() const {
    return view_items_.size();
}


void PreservedCommandListView::ResetHeight() {

    float new_height{};

    for (const auto& each_item : view_items_) {
        new_height += each_item->view->Height();
    }

    constexpr float max_height = 500;
    new_height = std::min(new_height, max_height);

    this->SetFixedHeight(new_height);
}

}