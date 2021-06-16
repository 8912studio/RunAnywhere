#include "utility/preview_text_box.h"
#include <zaf/graphic/color.h>
#include <zaf/reflection/reflection_type_definition.h>

namespace ra::utility {

ZAF_DEFINE_REFLECTION_TYPE(PreviewTextBox)
ZAF_DEFINE_END

void PreviewTextBox::AfterParse() {

	__super::AfterParse();
	
	this->SetBackgroundColor(zaf::Color::Transparent());
	this->SetBorder(0);
	this->SetIsReadOnly(true);
	this->SetAllowBeep(false);
	this->SetTextAlignment(zaf::TextAlignment::Center);
	this->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
	
	Subscriptions() += this->FocusChangeEvent().Subscribe(
		[this](const zaf::ControlFocusChangeInfo&) {

		if (!this->IsFocused()) {
			this->SetSelectionRange(zaf::TextRange{});
		}
	});
	
}

}