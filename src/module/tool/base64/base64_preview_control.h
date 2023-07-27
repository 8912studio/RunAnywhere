#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/common/binary_content/binary_content_control.h"
#include "module/common/content_status_bar.h"
#include "module/common/error_view.h"
#include "module/common/text_content_control.h"
#include "module/tool/base64/base64_command_parsing.h"

namespace ra::mod::tool::base64 {

class Base64PreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void ShowParseResult(const Base64CommandParseResult& parse_result);
    std::wstring GetResultText() const;

    zaf::Frame GetExpectedMargin() override;

protected:
    void OnStyleChanged() override;

private:
    void ShowEncodeResult(const Base64CommandParseResult& parse_result);
    void ShowDecodeResult(
        std::vector<std::byte> decoded_data, 
        const Base64CommandParseResult& parse_result);
    static std::optional<std::wstring> ConvertDecodedDataToText(
        const std::vector<std::byte>& decoded_data, 
        const Base64CommandParseResult& parse_result,
        TextEncoding& encoding);

    void ShowOperationType(Base64Operation operation);

    void ShowTextContent(
        const std::wstring& input_text, 
        TextEncoding encoding,
        std::wstring text_content,
        bool is_base64,
        const std::wstring& icon_tooltip,
        const std::wstring& encoding_tooltip);

    void ShowBinaryContent(const std::wstring& input_text, std::vector<std::byte> binary_content);

private:
    ZAF_BIND_CONTROL(zaf::Label, operationLabel);
    ZAF_BIND_CONTROL(ContentStatusBar, contentStatusBar);
    ZAF_BIND_CONTROL(TextContentControl, textContent);
    ZAF_BIND_CONTROL(BinaryContentControl, binaryContent);
    ZAF_BIND_CONTROL(ErrorView, errorView);
};

}