#include "main/toolbar/command_toolbar.h"
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(CommandToolbar)
ZAF_DEFINE_TYPE_END;

void CommandToolbar::SetStyle(CommandDisplayStyle style) {

    style_ = style;

    UpdateButtons();
}


void CommandToolbar::AfterParse() {

    __super::AfterParse();

    UpdateButtons();
}


void CommandToolbar::UpdateButtons() {

}

}