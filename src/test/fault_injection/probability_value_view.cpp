#include "test/fault_injection/probability_value_view.h"
#include "test/fault_injection/probability.h"

namespace ra::test {

ZAF_OBJECT_IMPL(ProbabilityValueView);

zaf::ObjectType* ProbabilityValueView::ValueType() {
    return Probability::StaticType();
}


std::shared_ptr<zaf::Object> ProbabilityValueView::GetValue() const {
    return nullptr;
}


void ProbabilityValueView::SetValue(const zaf::Object& value) {

}


void ProbabilityValueView::AfterParse() {

    __super::AfterParse();
}

}