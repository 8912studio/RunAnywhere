#include "module/tool/hex/hex_preview_control.h"
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include <fstream>

namespace ra::module::tool::hex {
namespace {

std::optional<std::vector<std::byte>> ReadFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result) {

    if (parse_result.length == 0) {
        return std::vector<std::byte>{};
    }

    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        file_stream.open(file_path, std::ifstream::in | std::ifstream::binary);
        file_stream.seekg(parse_result.position, std::ifstream::beg);

        auto buffer_length = std::min(parse_result.length, static_cast<std::size_t>(4 * 1024));
        std::vector<std::byte> buffer(buffer_length);
        file_stream.read(reinterpret_cast<char*>(&buffer[0]), buffer_length);
        return buffer;
    }
    catch (const std::ifstream::failure&) {
        return std::nullopt;
    }
}

}

ZAF_DEFINE_TYPE(HexPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/hex/hex_preview_control.xaml")
ZAF_DEFINE_TYPE_END;


void HexPreviewControl::AfterParse() {

    __super::AfterParse();

    auto scroll_bar = scrollControl->VerticalScrollBar();
    scroll_bar->SetArrowLength(0);
    scroll_bar->SetSmallChange(
        static_cast<int>(HexContentControl::LineHeight));
}


void HexPreviewControl::ShowFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result) {

    file_path_ = file_path;

    auto file_content = ReadFileContent(file_path, parse_result);
    if (file_content) {

        contentControl->SetContent(*file_content);
        contentControl->SetAutoHeight(true);

        this->SetFixedHeight(HexContentControl::LineHeight * 8);
    }
}


zaf::Frame HexPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 2;
    return result;
}

}