#pragma once

#include <zaf/base/none.h>
#include <zaf/control/control.h>

namespace ra::test {

class ValueView : public zaf::Control {
public:
    ZAF_OBJECT;

public:
    virtual std::shared_ptr<zaf::Object> GetValue() const = 0;
    virtual void SetValue(const zaf::Object& value) = 0;

    zaf::Observable<zaf::None> ValueChangedEvent() const {
        return value_changed_event_.AsObservable();
    }

protected:
    void AfterParse() override;

    void NotifyValueChanged() {
        value_changed_event_.AsObserver().OnNext({});
    }

private:
    zaf::Subject<zaf::None> value_changed_event_;
};

ZAF_OBJECT_BEGIN(ValueView);
ZAF_OBJECT_END;

}