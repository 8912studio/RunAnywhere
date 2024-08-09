#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/image_box.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include "module/command_preview_control.h"

namespace ra::mod {

class ErrorView : public zaf::HorizontalBox {
public:
    ZAF_OBJECT;

    void ShowErrorText(const std::wstring& text);
    void ShowHintText(const std::wstring& text);

    void ChangeStyle(CommandDisplayStyle style);

private:
    void ShowText(const std::wstring& text, bool is_error);

private:
    ZAF_BIND_CONTROL(zaf::ImageBox, errorIcon);
    ZAF_BIND_CONTROL(zaf::Label, textLabel);
};

ZAF_OBJECT_BEGIN(ErrorView)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/common/error_view.xaml")
ZAF_OBJECT_END;

}