#pragma once

#include "module/command.h"
#include "module/command_preview_control.h"

namespace ra {

std::shared_ptr<mod::CommandPreviewControl> CreateCommandPreviewControl(mod::Command& command);

}