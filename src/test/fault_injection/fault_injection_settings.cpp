#include "test/fault_injection//fault_injection_settings.h"

namespace ra::test {

ZAF_OBJECT_IMPL(FaultInjectionSettings);

std::shared_ptr<FaultInjectionSettings> FaultInjectionSettings::Instance() {
    static auto instance = zaf::Init(new FaultInjectionSettings{});
    return instance;
}

}