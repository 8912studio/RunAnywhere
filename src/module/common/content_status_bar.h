#pragma once

#include <filesystem>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include "module/common/text_encoding.h"

namespace ra::mod {

class ContentStatusBar : public zaf::HorizontalBox {
public:
    ZAF_OBJECT;

public:
    void ShowFile(const std::filesystem::path& file_path);
    void ShowText(const std::wstring& text, TextEncoding encoding);
    void ShowBinary(const std::wstring& binary_text);
    void ShowNone(const std::wstring& text);

    void SetIconTooltip(const std::wstring& tooltip);
    void SetEncodingTooltip(const std::wstring& tooltip);

private:
    void SetTypeIcon(const std::wstring& uri);
    void SetEncoding(std::optional<TextEncoding> encoding);
    void SetText(const std::wstring& text);

private:
    ZAF_BIND_CONTROL(zaf::Control, typeIcon);
    ZAF_BIND_CONTROL(zaf::Control, encodingContainer);
    ZAF_BIND_CONTROL(zaf::Label, encodingLabel);
    ZAF_BIND_CONTROL(zaf::Label, contentLabel);
};

ZAF_OBJECT_BEGIN(ContentStatusBar)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/common/content_status_bar.xaml")
ZAF_OBJECT_END;

}