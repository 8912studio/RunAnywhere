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