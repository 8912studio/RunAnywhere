#pragma once

#include <optional>
#include <zaf/control/clickable_control.h>
#include <zaf/rx/subject.h>
#include "hot_key.h"

namespace ra {

class HotKeyBox : public zaf::ClickableControl {
public:
    ZAF_OBJECT;

    HotKey GetHotKey() const {
        return hot_key_;
    }

    void SetHotKey(const HotKey& hot_key);

    zaf::Observable<HotKey> HotKeyChangedEvent() {
        return hot_key_changed_subject_.AsObservable();
    }

protected:
    void Initialize() override;
    void UpdateStyle() override;
    bool AcceptKeyMessage(const zaf::KeyMessage& message) override;
    void OnKeyDown(const zaf::KeyDownInfo& event_info) override;
    void OnFocusLost(const zaf::FocusLostInfo& event_info) override;
    void OnClick(const zaf::ClickInfo&) override;

private:
    void SetIsWaitingInput(bool value);
    void UpdateText();

private:
    HotKey hot_key_;
    bool is_waiting_input_{};

    zaf::Subject<HotKey> hot_key_changed_subject_;
};

ZAF_OBJECT_BEGIN(HotKeyBox);
ZAF_OBJECT_END;

}