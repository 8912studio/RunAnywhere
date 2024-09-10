#include "utility/preview_text_box.h"
#include <zaf/graphic/color.h>
#include "module/common/style_constants.h"

namespace ra::utility {

ZAF_OBJECT_IMPL(PreviewTextBox);

void PreviewTextBox::AfterParse() {

	__super::AfterParse();
	
	this->SetBackgroundColor(zaf::Color::Transparent());
	this->SetBorder(zaf::Frame{ 0 });
	this->SetIsEditable(false);
	this->SetTextAlignment(zaf::dwrite::TextAlignment::Center);
	this->SetParagraphAlignment(zaf::dwrite::ParagraphAlignment::Center);
	
	Subscriptions() += this->FocusLostEvent().Subscribe(std::bind([this]() {
		this->SetSelectionRange(zaf::Range{});
	}));
}


void PreviewTextBox::Display(CommandDisplayStyle style) {

	if (style == CommandDisplayStyle::Preserved) {
		this->SetTextAlignment(zaf::dwrite::TextAlignment::Left);
		this->SetFontSize(mod::StyleConstants::PreservedBodyFontSize);
	}
	else {
		this->SetTextAlignment(zaf::dwrite::TextAlignment::Center);
		this->SetFontSize(26);
	}
}

}