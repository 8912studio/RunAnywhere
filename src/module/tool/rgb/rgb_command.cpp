#include "module/tool/rgb/rgb_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::tool::rgb {

RGBCommand::RGBCommand(const RGBCommandParseResult& parse_result) {

    preview_control_ = zaf::Create<RGBPreviewControl>(parse_result);
}


help::content::Content RGBCommand::GetHelpContent() {

    help::content::Content result;

    result.AddTitleLine(L"RGB command");
    result.AddBodyLine(L"Show RGB color.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(
        L"rgb R,G,B\r\n"
        L"rgb R,G,B,A"
    );
    result.AddBodyLine(
        L"Show color specified with `R`, `G`, `B`, `A` components. Components can be following "
        L"forms:\r\n" 
        "- Integers, range from 0 to 255.\r\n"
        "- Floats, range from 0 to 1.0.\r\n"
        "- Hex, range from 0x0 to 0xFF.\r\n"
        "All components must be the same form.\r\n"
        "If `A` is omitted, the opacity is 100%."
    );
    result.AddBodyLine(
        L"rgb #RRGGBB\r\n"
        L"rgb #AARRGGBB");
    result.AddBodyLine(L"Show color specified with hex RGB or ARGB format.");

    result.AddBodyLine(
        L"rgb <...> $A\r\n"
        L"Specify additional alpha to the color. `A` can be integer, float or hex."
    );

    result.AddTitleLine(L"Switchs");
    result.AddTwoPartsLine(L"/a", L"Reserve opacity.");
    result.AddTwoPartsLine(L"/d", L"Show color components in integer form.");
    result.AddTwoPartsLine(L"/f", L"Show color components in float form.");
    result.AddTwoPartsLine(L"/x", L"Show color components in hex form.");

    result.AddTitleLine(L"Remarks");
    result.AddBodyLine(
        L"If the input color's opacity is not 100%, it will be transformed to another color which "
        "opacity is 100% by default.\r\n"
        "\r\n"
        "For example, an input color #CC000000 would be transformed to #FF333333 and shown in "
        "result.\r\n"
        "\r\n"
        "If you want to reserve its opacity, you can specify `/a` "
        "switch. However, the appearance of color will not changed, because its opacity is "
        "combined with the white background."
    );

    return result;
}


std::shared_ptr<CommandPreviewControl> RGBCommand::GetPreviewControl() {

    return preview_control_;
}


void RGBCommand::Execute() {

    utility::SetStringToClipboard(preview_control_->GetText());
}

}