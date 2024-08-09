#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include "main/preserve/preserved_command_view.h"
#include "utility/thin_scroll_control.h"

namespace ra {

class PreservedCommandListView : public zaf::Control {
public:
    ZAF_OBJECT;

    void AddView(const std::shared_ptr<PreservedCommandView>& view);
    void RemoveView(const std::shared_ptr<PreservedCommandView>& view);
    void RemoveFirstView();

    std::size_t ViewCount() const;

protected:
    void AfterParse() override;

private:
    class ViewItem {
    public:
        std::shared_ptr<PreservedCommandView> view;
        zaf::SubscriptionSet subscriptions;
    };

    void RemoveExcessViews();
    void OnViewStateUpdated(const std::shared_ptr<PreservedCommandView>& view);
    void ResetHeight();
    void ScrollToView(const std::shared_ptr<PreservedCommandView>& view);

    void RemoveFirstViewWithoutResetHeight();

private:
    ZAF_BIND_CONTROL(utility::ThinScrollControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::Control, listView);

    std::vector<std::unique_ptr<ViewItem>> view_items_;
};

ZAF_OBJECT_BEGIN(PreservedCommandListView)
ZAF_OBJECT_RESOURCE_URI(L"res:///main/preserve/preserved_command_list_view.xaml")
ZAF_OBJECT_END;

}