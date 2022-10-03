#include "module/tool/hex/hex_preview_control.h"
#include <zaf/object/type_definition.h>

namespace ra::module::tool::hex{

ZAF_DEFINE_TYPE(HexPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/hex/hex_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void HexPreviewControl::ShowFileContent(
    const std::filesystem::path& file_path,
    const HexCommandParseResult& parse_result) {


}

}