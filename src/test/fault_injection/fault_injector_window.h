#pragma once

#include <zaf/window/window.h>

namespace ra::test {

class FaultInjectorWindow : public zaf::Window {
public:
    static const std::shared_ptr<FaultInjectorWindow>& Instance();

protected:
    void AfterParse() override;

private:
    FaultInjectorWindow() = default;
};

}