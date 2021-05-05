#include "module/active_path/active_path_utility.h"

namespace ra::module::active_path {

std::wstring AdjustActivePathByOption(
	const std::filesystem::path& active_path,
	const ActivePathOption& option) {

    try {

        auto result = active_path;

        if (option.backward_level >= 1) {
            if (!std::filesystem::is_directory(result)) {
                result = result.parent_path();
            }
        }

        for (std::size_t level = 2; level <= option.backward_level; ++level) {
            result = result.parent_path();
        }

        if (option.name_only) {
            result = result.filename();
        }

        return result.wstring();
    }
    catch (const std::filesystem::filesystem_error&) {
        return {};
    }
}

}