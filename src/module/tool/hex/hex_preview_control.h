#pragma once

#include <filesystem>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"
#include "module/common/binary_content/binary_content_control.h"
#include "module/common/error_view.h"
#include "module/tool/hex/hex_command_parse_result.h"

namespace ra::mod::tool::hex {

class HexPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    enum class ReadFileStatus {
        OK,
        EmptyFile,
        ReadFileFailed,
        InvalidPosition,
    };

    class FileContentInfo {
    public:
        std::vector<std::byte> data;
        std::uint64_t file_size{};
    };

public:
    //For unit test.
    static ReadFileStatus ReadFileContent(
        const std::filesystem::path& file_path,
        const HexCommandParseResult& parse_result,
        FileContentInfo& content_info);

public:
    void ShowFileContent(
        const std::filesystem::path& file_path,
        const HexCommandParseResult& parse_result);

    zaf::Frame GetExpectedMargin() override;

protected:
    void AfterParse() override;
    void OnStyleChanged() override;

private:
    void ChangeControlStyles();
    void ShowFilePath(const std::filesystem::path& path);
    void ShowFileInfo(
        ReadFileStatus status, 
        const FileContentInfo& content_info,
        const HexCommandParseResult& parse_result);
    void ShowHexContent(const FileContentInfo& content_info);
    void ShowMessage(
        ReadFileStatus status,
        const FileContentInfo& content_info);

private:
    ZAF_BIND_CONTROL(zaf::Label, filePathLabel);
    ZAF_BIND_CONTROL(zaf::Label, fileInfoLabel);
    ZAF_BIND_CONTROL(BinaryContentControl, binaryContent);
    ZAF_BIND_CONTROL(ErrorView, errorView);
};

}