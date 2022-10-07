#pragma once

#include <filesystem>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"
#include "module/tool/hex/hex_command_parse_result.h"
#include "module/tool/hex/hex_content_control.h"

namespace ra::module::tool::hex {

class HexPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    enum class ReadFileStatus {
        OK,
        Error,
        InvalidPosition,
        NoContent,
    };

public:
    //For unit test.
    static ReadFileStatus ReadFileContent(
        const std::filesystem::path& file_path,
        const HexCommandParseResult& parse_result,
        std::vector<std::byte>& content);

public:
    void ShowFileContent(
        const std::filesystem::path& file_path,
        const HexCommandParseResult& parse_result);

    zaf::Frame GetExpectedMargin() override;

protected:
    void AfterParse() override;

private:
    void ShowHexContent(const std::vector<std::byte>& content);
    void ShowErrorMessage(ReadFileStatus status);

private:
    ZAF_BIND_CONTROL(zaf::Label, filePathLabel);
    ZAF_BIND_CONTROL(zaf::Control, contentContainer);
    ZAF_BIND_CONTROL(HexContentControl, contentControl);
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::Control, errorContainer);
    ZAF_BIND_CONTROL(zaf::Label, errorLabel);
};

}