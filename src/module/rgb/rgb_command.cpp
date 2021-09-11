#include "module/rgb/rgb_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::rgb {

RGBCommand::RGBCommand(const RGBCommandParseResult& parse_result) {

    preview_control_ = zaf::Create<RGBPreviewControl>(parse_result);
}


help::content::Content RGBCommand::GetHelpContent() {

    help::content::Content result;

    result.AddTitleLine(L"RGB command");
    result.AddBodyLine(L"Show color using RGB format.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(L"rgb <r,g,b>");
    result.AddBodyLine(L"`r`, `g`, `b` are decimal numbers, range from 0 to 255.");

    return result;
}


std::shared_ptr<CommandPreviewControl> RGBCommand::GetPreviewControl() {

    return preview_control_;
}


void RGBCommand::Execute() {

    utility::SetStringToClipboard(preview_control_->GetText());
}

}