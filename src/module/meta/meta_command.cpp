#include "module/meta/meta_command.h"
#include "module/meta/meta_command_prefix.h"

namespace ra::module::meta {

help::content::Content MetaCommand::GetHelpContent() {

    help::content::Content result;
    result.AddTitleLine(MetaCommandPrefix + command_info_.command + L" command");
    result.AddBodyLine(command_info_.description);
    return result;
}

}