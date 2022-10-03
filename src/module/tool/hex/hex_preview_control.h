#pragma once

#include <filesystem>
#include "module/command_preview_control.h"
#include "module/tool/hex/hex_command_parse_result.h"

namespace ra::module::tool::hex {

class HexPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void ShowFileContent(
        const std::filesystem::path& file_path,
        const HexCommandParseResult& parse_result);

private:
    std::filesystem::path file_path_;
};

}