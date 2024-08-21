#pragma once

#include <zaf/control/text_box.h>
#include "main/command_display_style.h"

namespace ra::utility {

/**
Used to display simple single line text.

TODO: move to module directory.
*/
class PreviewTextBox : public zaf::TextBox {
public:
	ZAF_OBJECT;

	void Display(CommandDisplayStyle style);

protected:
	void AfterParse() override;
};

ZAF_OBJECT_BEGIN(PreviewTextBox)
ZAF_OBJECT_END

}