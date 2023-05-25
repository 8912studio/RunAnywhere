#include "main/command_input_edit.h"
#include <zaf/base/clipboard.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/com_object.h>
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(CommandInputEdit)
ZAF_DEFINE_TYPE_END;

void CommandInputEdit::Initialize() {

    __super::Initialize();

    this->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    this->SetAcceptReturn(false);
}


utility::CommandLine CommandInputEdit::GetInputCommandLine() {

    auto ole_interface = this->GetOLEInterface();

    return utility::CommandLine(this->Text(), [ole_interface](int object_index) {

        try {

            auto object = ole_interface.GetObjectAt(object_index);
            auto text_block_object = object.As<TextBlockObject>();
            if (text_block_object) {
                return text_block_object->Text();
            }
            return std::wstring{};
        }
        catch (const zaf::Error&) {
            return std::wstring{};
        }
    });
}


zaf::Observable<zaf::None> CommandInputEdit::CommandChangedEvent() {
    return command_changed_event_.GetObservable();
}


void CommandInputEdit::OnTextChanging(const zaf::TextChangingInfo& event_info) {

    __super::OnTextChanging(event_info);

    if (event_info.Reason() == zaf::TextChangeReason::Paste) {

        if (TryToInsertTextBlockObject()) {
            event_info.SetCanChange(false);
        }
    }
}


bool CommandInputEdit::TryToInsertTextBlockObject() {

    auto text = zaf::Clipboard::GetText();
    if (!ShouldInsertTextBlockObject(text)) {
        return false;
    }

    InsertTextBlockObject(text);
    return true;
}


bool CommandInputEdit::ShouldInsertTextBlockObject(const std::wstring& text) {

    if (text.length() > 30) {
        return true;
    }

    if (zaf::Contain(text, L'\r') || zaf::Contain(text, L'\n')) {
        return true;
    }

    return false;
}


zaf::COMObject<TextBlockObject> CommandInputEdit::InsertTextBlockObject(const std::wstring& text) {

    auto text_block_object = zaf::MakeCOMObject<TextBlockObject>(text);
    Subscriptions() += text_block_object->TextChangedEvent().Subscribe(
        std::bind(&CommandInputEdit::RaiseCommandChangedEvent, this));

    this->InsertObject(text_block_object);
    return text_block_object;
}


void CommandInputEdit::OnTextChanged(const zaf::TextChangedInfo& event_info) {
    RaiseCommandChangedEvent();
}


void CommandInputEdit::RaiseCommandChangedEvent() {
    command_changed_event_.GetObserver().OnNext({});
}


void CommandInputEdit::OnSysKeyDown(const zaf::SysKeyDownInfo& event_info) {

    if (event_info.Message().VirtualKey() == L'T') {
        InsertTextBlockObjectByKey();
        event_info.MarkAsHandled();
    }

    __super::OnKeyDown(event_info);
}


void CommandInputEdit::InsertTextBlockObjectByKey() {

    auto object = InsertTextBlockObject({});

    auto selection_range = this->GetSelectionRange();
    if (selection_range.index <= 0) {
        //Abnormal situation.
        return;
    }

    //Select the inserted object.
    selection_range.index--;
    selection_range.length = 1;
    this->SetSelectionRange(selection_range);

    object->OpenWindow();
}

}