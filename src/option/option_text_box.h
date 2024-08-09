#pragma once

#include <zaf/control/rich_edit.h>
#include <zaf/rx/subject.h>

namespace ra::option {

class OptionTextBox : public zaf::RichEdit {
public:
    ZAF_OBJECT;

    zaf::Observable<std::wstring> ValueChangedEvent() const {
        return value_changed_event_.AsObservable();
    }

protected:
    void AfterParse() override;

private:
    void CommitValueChanged();

private:
    zaf::Subject<std::wstring> value_changed_event_;
    bool value_changed_{};
};

ZAF_OBJECT_BEGIN(OptionTextBox)
ZAF_OBJECT_END;

}