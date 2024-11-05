#include "test/fault_injection/fault_injector_window.h"
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/property_grid.h>
#include "test/fault_injection/fault_injection_settings.h"

namespace ra::test {

const std::shared_ptr<FaultInjectorWindow>& FaultInjectorWindow::Instance() {
    static auto instance = zaf::Init(new FaultInjectorWindow{});
    return instance;
}


void FaultInjectorWindow::AfterParse() {

    __super::AfterParse();

    this->RootControl()->SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    auto property_grid = zaf::Create<zaf::PropertyGrid>();
    property_grid->SetTargetObject(FaultInjectionSettings::Instance());

    this->RootControl()->AddChild(property_grid);

    /*
    for (auto each_property : FaultInjectionSettings::StaticType()->AllProperties()) {

        auto setting_item_view = zaf::Create<SettingItemView>(each_property);
        this->RootControl()->AddChild(setting_item_view);
    }
    */
}

}