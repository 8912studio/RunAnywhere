#include "module/active_path/active_path_modifying.h"

namespace ra::module::active_path {

context::ActivePath ModifyActivePathByOption(
    const context::ActivePath& active_path,
	const ActivePathOption& option) {

    try {

        std::filesystem::path new_path;

        if (option.use_workspace_path && active_path.HasWorkspacePath()) {
            new_path = active_path.GetWorkspacePath();
        }
        else {
            new_path = active_path.GetPath();
        }

        if (option.backward_level >= 1) {
            if (!std::filesystem::is_directory(new_path)) {
                new_path = new_path.parent_path();
            }
        }

        for (std::size_t level = 2; level <= option.backward_level; ++level) {
            new_path = new_path.parent_path();
        }

        if (option.use_name) {
            new_path = new_path.filename();
        }

        return context::ActivePath{ new_path, active_path.GetWorkspacePath() };
    }
    catch (const std::filesystem::filesystem_error&) {
        return {};
    }
}

}