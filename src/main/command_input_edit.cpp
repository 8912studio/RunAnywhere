#include "main/command_input_edit.h"
#include <zaf/base/clipboard.h>
#include <zaf/base/com_object.h>
#include <zaf/object/type_definition.h>
#include "main/text_block_object.h"

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


void CommandInputEdit::OnTextChanging(const zaf::TextChangingInfo& event_info) {

    __super::OnTextChanging(event_info);

    if (event_info.Reason() == zaf::TextChangeReason::Paste) {

        auto text = zaf::Clipboard::GetText();
        if (!text.empty()) {

            auto text_block_object = zaf::MakeCOMObject<TextBlockObject>(text);
            this->InsertObject(text_block_object);

            event_info.SetCanChange(false);
        }
    }
}

}