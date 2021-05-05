#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_utility.h"
#include "utility/clipboard.h"

namespace ra::module::active_path {

ActivePathCommand::ActivePathCommand(const ActivePathOption& option) : option_(option) {

}


std::wstring ActivePathCommand::GetPreviewText() {

	return AdjustActivePathByOption(GetDesktopContext().active_path, option_);
}


void ActivePathCommand::Execute() {

	utility::SetStringToClipboard(GetPreviewText());
}

}