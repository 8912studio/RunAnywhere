#pragma once

#include <zaf/control/property_grid/type_config.h>

namespace ra::test {

class ProbabilityTypeConfig : public zaf::property_grid::TypeConfig {
public:
    std::shared_ptr<zaf::property_grid::ValueView> CreateValueView() override;
};

}