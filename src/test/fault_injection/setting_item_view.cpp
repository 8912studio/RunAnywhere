#include "test/fault_injection/setting_item_view.h"
#include <zaf/base/as.h>
#include "test/fault_injection/fault_injection_settings.h"
#include "test/fault_injection/probability_value_view.h"

namespace ra::test {

ZAF_OBJECT_IMPL(SettingItemView);

SettingItemView::SettingItemView(zaf::ObjectProperty* property) : property_(property) {

}


void SettingItemView::AfterParse() {

    __super::AfterParse();

    value_view_ = CreateValueView();
    ZAF_EXPECT(value_view_);

    auto value = property_->GetValue(*FaultInjectionSettings::Instance());
    value_view_->SetValue(*value);

    Subscriptions() += value_view_->ValueChangedEvent().Subscribe(std::bind([this]() {
        auto value = value_view_->GetValue();
        property_->SetValue(*FaultInjectionSettings::Instance(), value);
    }));

    valueViewContainer->AddChild(value_view_);
}


std::shared_ptr<ValueView> SettingItemView::CreateValueView() const {

    struct MapItem {
        zaf::ObjectType* value_type;
        zaf::ObjectType* view_type;
    };
    static const MapItem map[] = {
        { ProbabilityValueView::ValueType(), ProbabilityValueView::StaticType() },
    };

    for (const auto& each_item : map) {

        if (map->value_type == property_->ValueType()) {
            return zaf::As<ValueView>(map->value_type->CreateInstance());
        }
    }

    return nullptr;
}

}