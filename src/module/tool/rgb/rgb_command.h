#pragma once

#include "module/command.h"
#include "module/tool/rgb/rgb_command_parse_result.h"
#include "module/tool/rgb/rgb_preview_control.h"

namespace ra::module::tool::rgb {

class RGBCommand : public Command {
public:
    RGBCommand(const RGBCommandParseResult& parse_result);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::shared_ptr<RGBPreviewControl> preview_control_;
};

}