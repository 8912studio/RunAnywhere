#pragma once

#include "help/content/content.h"
#include "module/command_brief.h"

namespace ra::help {

content::Content BuildHelpContentFromSuggestedCommands(
    std::vector<module::CommandBrief>&& commands);

}