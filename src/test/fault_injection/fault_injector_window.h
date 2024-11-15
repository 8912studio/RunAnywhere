#pragma once

#include <zaf/window/window.h>
#include <zaf/control/property_grid_delegate.h>

namespace ra::test {

class FaultInjectorWindow : public zaf::Window, public zaf::PropertyGridDelegate {
public:
    static const std::shared_ptr<FaultInjectorWindow>& Instance();

    void ShowWindow();

protected:
    void AfterParse() override;

private:
    std::shared_ptr<zaf::property_grid::TypeConfig> GetTypeConfig(zaf::ObjectType* type) override;

private:
    FaultInjectorWindow() = default;
};

}