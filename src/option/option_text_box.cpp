#include "option/option_text_box.h"

namespace ra::option {

ZAF_OBJECT_IMPL(OptionTextBox);

void OptionTextBox::AfterParse() {

    __super::AfterParse();

    this->SetIsEditable(true);
    this->SetIsMultiline(false);
    this->SetBorder(zaf::Frame{ 1 });

    Subscriptions() += TextChangedEvent().Subscribe(std::bind([this]() {
        value_changed_ = true;
    }));

    Subscriptions() += FocusLostEvent().Subscribe(std::bind([this]() {
        CommitValueChanged();
    }));

    Subscriptions() += KeyDownEvent().Subscribe([this](const zaf::KeyDownInfo& event_info) {
        if (event_info.Message().Key() == zaf::Key::Enter) {
            CommitValueChanged();
            event_info.MarkAsHandled();
        }
    });
}


void OptionTextBox::CommitValueChanged() {
    if (value_changed_) {
        value_changed_event_.AsObserver().OnNext(Text());
        value_changed_ = false;
    }
}

}