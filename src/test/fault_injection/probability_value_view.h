#pragma once

#include "test/fault_injection/value_view.h"

namespace ra::test {

class ProbabilityValueView : public ValueView {
public:
    ZAF_OBJECT;

    static zaf::ObjectType* ValueType();

public:
    std::shared_ptr<zaf::Object> GetValue() const override;
    void SetValue(const zaf::Object& value) override;

protected:
    void AfterParse() override;
};

ZAF_OBJECT_BEGIN(ProbabilityValueView);
ZAF_OBJECT_RESOURCE_URI(L"res:///test/fault_injection/probability_value_view.xaml");
ZAF_OBJECT_END;

}