#pragma once

#include <zaf/object/object.h>
#include <zaf/object/property_support.h>
#include "test/fault_injection/probability.h"

namespace ra::test {

class FaultInjectionSettings : public zaf::Object {
public:
    ZAF_OBJECT;

    static std::shared_ptr<FaultInjectionSettings> Instance();

    Probability NetworkFailureProbability() const noexcept {
        return network_failure_probability_;
    }

    void SetNetworkFailureProbability(Probability value) noexcept {
        network_failure_probability_ = value;
    }

    bool UseMockResponse() const noexcept {
        return use_mock_response_;
    }

    void SetUseMockResponse(bool value) {
        use_mock_response_ = value;
    }

private:
    FaultInjectionSettings() = default;

private:
    Probability network_failure_probability_{};
    bool use_mock_response_{};
};

ZAF_OBJECT_BEGIN(FaultInjectionSettings);
ZAF_OBJECT_PROPERTY(NetworkFailureProbability);
ZAF_OBJECT_PROPERTY(UseMockResponse);
ZAF_OBJECT_END;

}