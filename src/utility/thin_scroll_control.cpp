#include "utility/thin_scroll_control.h"
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include "utility/thin_scroll_bar.h"

namespace ra::utility {

ZAF_DEFINE_TYPE(ThinScrollControl)
ZAF_DEFINE_TYPE_END;

void ThinScrollControl::AfterParse() {

    __super::AfterParse();

    this->SetBorder({});

    this->SetVerticalScrollBar(zaf::Create<ThinScrollBar>());
    this->SetHorizontalScrollBar(zaf::Create<ThinScrollBar>());
}

}