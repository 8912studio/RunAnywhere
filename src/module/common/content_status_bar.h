#pragma once

#include <filesystem>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include "module/common/text_encoding.h"

namespace ra::mod {

class ContentStatusBar : public zaf::HorizontalBox {
public:
    ZAF_DECLARE_TYPE;

public:
    void ShowFile(const std::filesystem::path& file_path);
    void ShowText(const std::wstring& text, TextEncoding encoding);

private:
    void SetTypeIcon(const std::wstring& uri);
    void SetEncoding(std::optional<TextEncoding> encoding);

private:
    ZAF_BIND_CONTROL(zaf::Control, typeIcon);
    ZAF_BIND_CONTROL(zaf::Control, encodingContainer);
    ZAF_BIND_CONTROL(zaf::Label, encodingLabel);
    ZAF_BIND_CONTROL(zaf::Label, contentLabel);
};

}