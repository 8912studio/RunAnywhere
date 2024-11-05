#include "test/fault_injection/fault_injection_type_config_factory.h"
#include "test/fault_injection/probability.h"
#include "test/fault_injection/probability_type_config.h"

using namespace zaf;
using namespace zaf::property_grid;

namespace ra::test {

std::shared_ptr<TypeConfig> FaultInjectionTypeConfigFactory::GetConfig(ObjectType* type) {

    if (type == Probability::StaticType()) {
        return std::make_shared<ProbabilityTypeConfig>();
    }

    return __super::GetConfig(type);
}

}