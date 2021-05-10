#pragma once

#include <filesystem>
#include "context/active_path.h"
#include "module/active_path/active_path_option.h"

namespace ra::module::active_path {

context::ActivePath ModifyActivePathByOption(
	const context::ActivePath& active_path,
	const ActivePathOption& option);

}