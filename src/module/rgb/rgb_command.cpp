#include "module/rgb/rgb_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::rgb {

RGBCommand::RGBCommand(const RGBCommandParseResult& parse_result) {

    preview_control_ = zaf::Create<RGBPreviewControl>(parse_result);
}


std::shared_ptr<CommandPreviewControl> RGBCommand::GetPreviewControl() {

    return preview_control_;
}


void RGBCommand::Execute() {

    utility::SetStringToClipboard(preview_control_->GetText());
}

}