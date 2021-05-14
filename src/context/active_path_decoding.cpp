#include "context/active_path_decoding.h"
#include <zaf/base/string/split.h>

namespace ra::context {

ActivePath DecodeActivePath(const std::wstring& encoded_path) {

    auto paths = zaf::Split(encoded_path, L'|');
    if (paths.empty()) {
        return {};
    }

    const auto& path = paths[0];
    if (path.empty()) {
        return {};
    }

    std::filesystem::path workspace_path;
    if (paths.size() > 1) {
        workspace_path = paths[1];
    }

    return ActivePath{ path, workspace_path };
}

}