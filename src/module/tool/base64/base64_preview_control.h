#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/common/binary_content/binary_content_control.h"
#include "module/common/content_status_bar.h"
#include "module/common/text_content_control.h"
#include "module/tool/base64/base64_command_parsing.h"

namespace ra::mod::tool::base64 {

class Base64PreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void ShowParseResult(const Base64CommandParseResult& parse_result);

    zaf::Frame GetExpectedMargin() override;

private:
    void ShowEncodeResult(const Base64CommandParseResult& parse_result);
    void ShowDecodeResult(
        std::vector<std::byte> decoded_data, 
        const Base64CommandParseResult& parse_result);
    static std::optional<std::wstring> ConvertDecodedDataToText(
        const std::vector<std::byte>& decoded_data, 
        const Base64CommandParseResult& parse_result,
        TextEncoding& encoding);

private:
    ZAF_BIND_CONTROL(ContentStatusBar, contentStatusBar);
    ZAF_BIND_CONTROL(TextContentControl, textContent);
    ZAF_BIND_CONTROL(BinaryContentControl, binaryContent);
};

}