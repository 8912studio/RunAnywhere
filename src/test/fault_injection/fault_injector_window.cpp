#include "test/fault_injection/fault_injector_window.h"
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/property_grid.h>
#include <zaf/graphic/dpi.h>
#include "test/fault_injection/fault_injection_settings.h"
#include "test/fault_injection/probability_type_config.h"

namespace ra::test {

const std::shared_ptr<FaultInjectorWindow>& FaultInjectorWindow::Instance() {
    static auto instance = zaf::Init(new FaultInjectorWindow{});
    return instance;
}


void FaultInjectorWindow::ShowWindow() {

    POINT mouse_position{};
    GetCursorPos(&mouse_position);
    auto monitor_handle = MonitorFromPoint(mouse_position, MONITOR_DEFAULTTONEAREST);

    MONITORINFO monitor_info{};
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfo(monitor_handle, &monitor_info);

    auto work_rect = zaf::Rect::FromRECT(monitor_info.rcWork);
    work_rect = zaf::ToDIPs(work_rect, this->GetDPI());

    this->SetInitialRectStyle(zaf::InitialRectStyle::Custom);
    zaf::Rect window_rect;
    window_rect.size = zaf::Size{ 500, work_rect.size.height };
    window_rect.position.x = work_rect.Right() - window_rect.size.width;

    this->SetRect(window_rect);
    this->Show();
}


void FaultInjectorWindow::AfterParse() {

    __super::AfterParse();

    this->RootControl()->SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    auto property_grid = zaf::Create<zaf::PropertyGrid>();
    property_grid->SetBorder({});
    property_grid->SetDelegate(zaf::As<PropertyGridDelegate>(shared_from_this()));
    property_grid->SetTargetObject(FaultInjectionSettings::Instance());

    this->RootControl()->AddChild(property_grid);
}


std::shared_ptr<zaf::property_grid::TypeConfig> FaultInjectorWindow::GetTypeConfig(
    zaf::ObjectType* type) {

    if (type == Probability::StaticType()) {
        return std::make_shared<ProbabilityTypeConfig>();
    }

    return __super::GetTypeConfig(type);
}

}