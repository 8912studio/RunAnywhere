#include "output_view.h"
#include <zaf/reflection/reflection_type_definition.h>

ZAF_DEFINE_REFLECTION_TYPE(OutputView)
    ZAF_DEFINE_RESOURCE_URI(L"res:///output_view.xaml");
ZAF_DEFINE_END


void OutputView::SetText(const std::wstring& text) {

    outputLabel->SetText(text);
}