#include "module/common/base_preview_control.h"
#include <zaf/object/type_definition.h>

namespace ra::module {

ZAF_DEFINE_TYPE(BasePreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/base_preview_control.xaml");
ZAF_DEFINE_TYPE_END;

void BasePreviewControl::ShowInvalidArgumentErrorView() {

    errorView->SetErrorText(L"Invalid argument");
    errorView->SetIsVisible(true);
}

}