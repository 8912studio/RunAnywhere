#include "module/tool/hex/hex_preview_control.h"
#include <zaf/object/type_definition.h>
#include <fstream>

namespace ra::module::tool::hex {
namespace {

std::optional<std::vector<std::byte>> ReadFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result) {

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

void HexPreviewControl::ShowFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result) {

    file_path_ = file_path;

    auto file_content = ReadFileContent(file_path, parse_result);
    if (file_content) {
        contentControl->SetContent(*file_content);
    }
}

}