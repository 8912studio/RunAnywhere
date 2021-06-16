#pragma once

#include <zaf/control/text_box.h>

namespace ra::utility {

class PreviewTextBox : public zaf::TextBox {
public:
	ZAF_DECLARE_REFLECTION_TYPE;

protected:
	void AfterParse() override;
};

}