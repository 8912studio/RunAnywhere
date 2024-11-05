#pragma once

#include <zaf/object/object.h>
#include <zaf/object/property_support.h>
#include "test/fault_injection/probability.h"

namespace ra::test {

class FaultInjectionSettings : public zaf::Object {
public:
    ZAF_OBJECT;

    static std::shared_ptr<FaultInjectionSettings> Instance();

    std::size_t NetworkFailureProbability() const noexcept {
        return network_failure_probability_;
    }

    void SetNetworkFailureProbability(std::size_t value) noexcept {
        network_failure_probability_ = value;
    }

private:
    FaultInjectionSettings() = default;

private:
    std::size_t network_failure_probability_{};
};

ZAF_OBJECT_BEGIN(FaultInjectionSettings);
ZAF_OBJECT_PROPERTY(NetworkFailureProbability);
ZAF_OBJECT_END;

}