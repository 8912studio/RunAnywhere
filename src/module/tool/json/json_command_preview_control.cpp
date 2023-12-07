#include "module/tool/json/json_command_preview_control.h"
#include <zaf/object/type_definition.h>

namespace ra::mod::tool::json {

ZAF_DEFINE_TYPE(JSONCommandPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/json/json_command_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void JSONCommandPreviewControl::AfterParse() {

    __super::AfterParse();
}


void JSONCommandPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    ResetHeight();
}


void JSONCommandPreviewControl::SetText(const utility::markdown::render::StyledText& text) {

    contentControl->SetStyledText(text);
}


void JSONCommandPreviewControl::OnStyleChanged() {
    ResetHeight();
}


void JSONCommandPreviewControl::ResetHeight() {

    auto content_size = contentControl->CalculatePreferredSize();
    if (Style() == CommandDisplayStyle::Preserved) {
        scrollControl->SetFixedHeight(content_size.height);
    }
    else {

        constexpr float min_height = 80;
        constexpr int max_line_count = 10;

        auto line_height = content_size.height / contentControl->LineCount();
        auto max_height = line_height * max_line_count;

        auto height = std::min(content_size.height, max_height);
        height = std::max(height, min_height);
        scrollControl->SetFixedHeight(height);
    }
}


zaf::Frame JSONCommandPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 0;
    result.top = 4;
    result.bottom = 6;
    return result;
}

}