#pragma once

#include <zaf/control/rich_edit.h>
#include "module/command_preview_control.h"

namespace ra::utility {

/**
Used to display simple single line text.

TODO: move to module directory.
*/
class PreviewTextBox : public zaf::RichEdit {
public:
	ZAF_DECLARE_TYPE;

	void Display(mod::PreviewStyle style);

protected:
	void AfterParse() override;
};

}