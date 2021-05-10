#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_modifying.h"
#include "utility/clipboard.h"

namespace ra::module::active_path {

ActivePathCommand::ActivePathCommand(const ActivePathOption& option) : option_(option) {

}


std::wstring ActivePathCommand::GetPreviewText() {

	auto new_active_path = ModifyActivePathByOption(GetDesktopContext().active_path, option_);
	return new_active_path.GetPath().wstring();
}


void ActivePathCommand::Execute() {

	utility::SetStringToClipboard(GetPreviewText());
}

}