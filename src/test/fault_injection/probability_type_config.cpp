#include "test/fault_injection/probability_type_config.h"
#include "test/fault_injection/probability_editor.h"

using namespace zaf;
using namespace zaf::property_grid;

namespace ra::test {

std::shared_ptr<zaf::property_grid::ValueEditor> ProbabilityTypeConfig::CreateValueEditor() {
    return zaf::Create<ProbabilityEditor>();
}

}