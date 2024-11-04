#include "test/fault_injection/value_view.h"

namespace ra::test {

ZAF_OBJECT_IMPL(ValueView);

void ValueView::AfterParse() {

    __super::AfterParse();
}

}