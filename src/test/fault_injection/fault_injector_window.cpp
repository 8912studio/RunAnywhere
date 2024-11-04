#include "test/fault_injection/fault_injector_window.h"
#include <zaf/control/layout/linear_layouter.h>
#include "test/fault_injection/fault_injection_settings.h"
#include "test/fault_injection/setting_item_view.h"

namespace ra::test {

const std::shared_ptr<FaultInjectorWindow>& FaultInjectorWindow::Instance() {
    static auto instance = zaf::Init(new FaultInjectorWindow{});
    return instance;
}


void FaultInjectorWindow::AfterParse() {

    __super::AfterParse();

    this->RootControl()->SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    for (auto each_property : FaultInjectionSettings::StaticType()->AllProperties()) {

        auto setting_item_view = zaf::Create<SettingItemView>(each_property);
        this->RootControl()->AddChild(setting_item_view);
    }
}

}