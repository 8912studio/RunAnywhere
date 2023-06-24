#include "module/common/binary_content/binary_content_control.h"
#include <zaf/control/scroll_bar.h>
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/object/type_definition.h>

namespace ra::mod {

ZAF_DEFINE_TYPE(BinaryContentControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/binary_content/binary_content_control.xaml")
ZAF_DEFINE_TYPE_END;

void BinaryContentControl::AfterParse() {

    __super::AfterParse();

    scrollControl->SetFixedHeight(LineHeight * 8);

    auto scroll_bar = scrollControl->VerticalScrollBar();
    scroll_bar->SetSmallChange(static_cast<int>(LineHeight));

    //If there is too much data, cached painting will fail, so we have to disable it.
    scrollControl->ScrollContent()->SetIsCachedPaintingEnabled(false);
}


void BinaryContentControl::SetBinary(std::vector<std::byte> binary) {
    body->SetBinary(std::move(binary));
}

}