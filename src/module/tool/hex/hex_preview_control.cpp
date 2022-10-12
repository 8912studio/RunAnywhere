#include "module/tool/hex/hex_preview_control.h"
#include <fstream>
#include <zaf/control/scroll_bar.h>
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/object/type_definition.h>
#include "module/tool/hex/paint_common.h"
#include "utility/numeric_text_formatting.h"
#include "utility/path_trimming.h"

namespace ra::module::tool::hex {
namespace {

std::wstring FormatInteger(std::uint64_t integer) {
    auto result = std::to_wstring(integer);
    utility::InsertSeparatorToNumericText(result, 10, L',');
    return result;
}

}

ZAF_DEFINE_TYPE(HexPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/hex/hex_preview_control.xaml")
ZAF_DEFINE_TYPE_END;


void HexPreviewControl::AfterParse() {

    __super::AfterParse();

    filePathLabel->SetTextTrimming(utility::CreateTextTrimmingForPath());

    scrollControl->SetFixedHeight(LineHeight * 8);

    auto scroll_bar = scrollControl->VerticalScrollBar();
    scroll_bar->SetArrowLength(0);
    scroll_bar->SetSmallChange(static_cast<int>(LineHeight));
    scroll_bar->GetThumb()->SetPadding({});
}


void HexPreviewControl::ShowFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result) {

    auto update_guard = BeginUpdate();

    ShowFilePath(file_path);

    FileContentInfo content_info;
    auto read_file_status = ReadFileContent(file_path, parse_result, content_info);

    ShowFileInfo(read_file_status, content_info, parse_result);
    ShowHexContent(content_info);
    ShowMessage(read_file_status, content_info);
}


void HexPreviewControl::ShowFilePath(const std::filesystem::path& path) {

    if (!path.empty()) {
        filePathLabel->SetText(path.wstring());
        filePathLabel->SetIsVisible(true);
    }
    else {
        filePathLabel->SetIsVisible(false);
    }
}


void HexPreviewControl::ShowFileInfo(
    ReadFileStatus status,
    const FileContentInfo& content_info,
    const HexCommandParseResult& parse_result) {

    if (status == ReadFileStatus::ReadFileFailed) {
        fileInfoLabel->SetIsVisible(false);
        return;
    }

    fileInfoLabel->SetIsVisible(true);

    std::wstring text;
    text += L"File size: ";
    text += FormatInteger(content_info.file_size);

    if (status == ReadFileStatus::OK) {

        auto range_end = parse_result.position;
        if (!content_info.data.empty()) {
            range_end += content_info.data.size() - 1;
        }

        text += L"    Range: ";
        text += FormatInteger(parse_result.position);
        text += L'-';
        text += FormatInteger(range_end);

        text += L"    Length: ";
        text += FormatInteger(content_info.data.size());
    }

    fileInfoLabel->SetText(text);
}


void HexPreviewControl::ShowHexContent(const FileContentInfo& content_info) {

    if (!content_info.data.empty()) {
        contentControl->SetContent(content_info.data);
        contentContainer->SetIsVisible(true);
    }
    else {
        contentContainer->SetIsVisible(false);
    }
}


void HexPreviewControl::ShowMessage(
    ReadFileStatus status, 
    const FileContentInfo& content_info) {

    std::wstring message;
    switch (status) {
    case ReadFileStatus::InvalidPosition:
        message = L"Position exceeds file size";
        break;
    case ReadFileStatus::ReadFileFailed:
        message = L"Unable to read file";
        break;
    default:
        if (content_info.data.empty()) {
            message = L"No content to display";
        }
        break;
    }

    if (!message.empty()) {
        errorLabel->SetText(message);
        errorContainer->SetIsVisible(true);
    }
    else {
        errorContainer->SetIsVisible(false);
    }
}


zaf::Frame HexPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.left = 2;
    result.right = 2;
    return result;
}


HexPreviewControl::ReadFileStatus HexPreviewControl::ReadFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result,
    FileContentInfo& content_info) {

    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        file_stream.open(file_path, std::ifstream::in | std::ifstream::binary);

        file_stream.seekg(0, std::ifstream::end);
        content_info.file_size = file_stream.tellg();
        if (content_info.file_size == 0) {
            return ReadFileStatus::EmptyFile;
        }

        if (parse_result.position >= content_info.file_size) {
            return ReadFileStatus::InvalidPosition;
        }

        auto can_read_length = 
            content_info.file_size - static_cast<std::streampos>(parse_result.position);

        //Change read length to default length if it is 0, which has no meaning.
        auto expected_read_length = 
            parse_result.length > 0 ? parse_result.length : HexCommandParseResult::DefaultLength;

        auto buffer_length = std::min({ 
            static_cast<std::streampos>(can_read_length),
            static_cast<std::streampos>(expected_read_length),
            static_cast<std::streampos>(HexCommandParseResult::MaxLength)
        });

        //No need to read file if length is 0.
        if (buffer_length == 0) {
            return ReadFileStatus::OK;
        }

        content_info.data.resize(buffer_length);

        file_stream.seekg(parse_result.position, std::ifstream::beg);
        file_stream.read(reinterpret_cast<char*>(&content_info.data[0]), buffer_length);

        return ReadFileStatus::OK;
    }
    catch (const std::ifstream::failure&) {
        return ReadFileStatus::ReadFileFailed;
    }
}

}