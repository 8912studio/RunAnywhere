#pragma once

#include <zaf/control/rich_edit.h>

namespace ra::utility {

class PreviewTextBox : public zaf::RichEdit {
public:
	ZAF_DECLARE_TYPE;

protected:
	void AfterParse() override;
};

}