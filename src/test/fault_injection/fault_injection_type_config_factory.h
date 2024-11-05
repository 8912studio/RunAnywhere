#pragma once

#include <zaf/control/property_grid/type_config_factory.h>

namespace ra::test {

class FaultInjectionTypeConfigFactory : public zaf::property_grid::TypeConfigFactory {
public:
    std::shared_ptr<zaf::property_grid::TypeConfig> GetConfig(zaf::ObjectType* type) override;
};

}