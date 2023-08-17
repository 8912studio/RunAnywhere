#pragma once

#include <filesystem>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"
#include "module/common/binary_content/binary_content_control.h"
#include "module/common/content_status_bar.h"
#include "module/common/error_view.h"
#include "module/common/general_input.h"
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
        const zaf::Range& range,
        FileContentInfo& content_info);

public:
    void ShowContent(const GeneralInput& input, const zaf::Range& file_range);

    //For unit test.
    const std::vector<std::byte>& GetBinary() const;

    zaf::Frame GetExpectedMargin() override;

protected:
    void OnStyleChanged() override;

private:
    void ShowTextContent(const std::wstring& text, TextEncoding encoding);
    std::vector<std::byte> CreateTextBinary(const std::wstring& text, TextEncoding encoding);
    void ShowTextInfo(const std::wstring& text, const std::vector<std::byte>& binary);

    void ShowFileContent(
        const std::filesystem::path& file_path,
        const zaf::Range& range);
    void ShowFileInfo(
        ReadFileStatus status, 
        const FileContentInfo& content_info,
        const zaf::Range& range);
    void ShowBinary(const std::vector<std::byte>& binary);
    void ShowMessage(
        ReadFileStatus status,
        const FileContentInfo& content_info);

private:
    ZAF_BIND_CONTROL(ContentStatusBar, contentStatusBar);
    ZAF_BIND_CONTROL(zaf::Label, fileInfoLabel);
    ZAF_BIND_CONTROL(BinaryContentControl, binaryContent);
    ZAF_BIND_CONTROL(ErrorView, errorView);
};

}