#include "module/tool/hex/hex_preview_control.h"
#include <fstream>
#include <zaf/control/scroll_bar.h>
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/object/type_definition.h>
#include "module/tool/hex/paint_common.h"
#include "utility/path_trimming.h"

namespace ra::module::tool::hex {

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

    filePathLabel->SetText(file_path.wstring());

    std::vector<std::byte> file_content;
    auto read_file_status = ReadFileContent(file_path, parse_result, file_content);
    if (read_file_status == ReadFileStatus::OK) {
        ShowHexContent(file_content);
    }
    else {
        ShowErrorMessage(read_file_status);
    }
}


void HexPreviewControl::ShowHexContent(const std::vector<std::byte>& content) {

    contentContainer->SetIsVisible(true);
    errorContainer->SetIsVisible(false);

    contentControl->SetContent(content);
}


void HexPreviewControl::ShowErrorMessage(ReadFileStatus status) {

    errorContainer->SetIsVisible(true);
    contentContainer->SetIsVisible(false);
    
    std::wstring error_message;
    switch (status) {
    case ReadFileStatus::InvalidPosition:
        error_message = L"Position exceeds file length";
        break;
    case ReadFileStatus::NoContent:
        error_message = L"No file content to display";
        break;
    default:
        error_message = L"Unable to read file";
        break;
    }

    errorLabel->SetText(error_message);
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
    std::vector<std::byte>& content) {

    if (parse_result.length == 0) {
        return ReadFileStatus::NoContent;
    }

    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        file_stream.open(file_path, std::ifstream::in | std::ifstream::binary);

        file_stream.seekg(0, std::ifstream::end);
        auto file_length = file_stream.tellg();
        if (file_length == 0) {
            return ReadFileStatus::NoContent;
        }

        if (static_cast<std::streampos>(parse_result.position) >= file_length) {
            return ReadFileStatus::InvalidPosition;
        }

        auto can_read_length = file_length - static_cast<std::streampos>(parse_result.position);

        auto buffer_length = std::min({ 
            static_cast<std::streampos>(can_read_length),
            static_cast<std::streampos>(parse_result.length),
            static_cast<std::streampos>(4 * 1024)
        });

        content.resize(buffer_length);

        file_stream.seekg(parse_result.position, std::ifstream::beg);
        file_stream.read(reinterpret_cast<char*>(&content[0]), buffer_length);

        return ReadFileStatus::OK;
    }
    catch (const std::ifstream::failure&) {
        return ReadFileStatus::Error;
    }
}

}