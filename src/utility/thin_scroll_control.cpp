#include "utility/thin_scroll_control.h"
#include <zaf/creation.h>
#include "utility/thin_scroll_bar.h"

namespace ra::utility {

ZAF_OBJECT_IMPL(ThinScrollControl);

void ThinScrollControl::AfterParse() {

    __super::AfterParse();

    this->SetBorder({});
    this->SetScrollBarThickness(10);
    this->SetVerticalScrollBar(zaf::Create<ThinScrollBar>());
    this->SetHorizontalScrollBar(zaf::Create<ThinScrollBar>());
}

}