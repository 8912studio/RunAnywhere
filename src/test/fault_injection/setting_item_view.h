#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include "test/fault_injection/value_view.h"

namespace ra::test {

class SettingItemView : public zaf::Control {
public:
    ZAF_OBJECT;

public:
    explicit SettingItemView(zaf::ObjectProperty* property);

    zaf::ObjectProperty* Property() const noexcept {
        return property_;
    }

protected:
    void AfterParse() override;

private:
    std::shared_ptr<ValueView> CreateValueView() const;

private:
    ZAF_BIND_CONTROL(zaf::Control, valueViewContainer);

    zaf::ObjectProperty* property_{};
    std::shared_ptr<ValueView> value_view_;
};

ZAF_OBJECT_BEGIN(SettingItemView);
ZAF_OBJECT_RESOURCE_URI(L"res:///test/fault_injection/setting_item_view.xaml");
ZAF_OBJECT_END;

}