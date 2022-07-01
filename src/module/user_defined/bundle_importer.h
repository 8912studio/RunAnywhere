#pragma once

#include <filesystem>
#include <zaf/base/non_copyable.h>
#include "module/user_defined/bundle_depot.h"

namespace ra::module::user_defined {

class BundleImporter : zaf::NonCopyable {
public:
    enum class State {
        Pending,
        Success,
        Fail,
    };

public:
    explicit BundleImporter(
        const std::shared_ptr<BundleDepot>& depot,
        const std::filesystem::path& target_directory_path,
        const std::filesystem::path& bundle_path);

    void Import();

private:
    bool ParseBundle();
    void SaveBundle();

private:
    std::weak_ptr<BundleDepot> depot_;
    std::filesystem::path target_directory_path_;
    std::filesystem::path bundle_path_;
    State state_{ State::Pending };
    std::shared_ptr<Bundle> parsed_bundle_;
};

}