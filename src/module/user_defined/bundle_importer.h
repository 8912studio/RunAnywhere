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
        OverrideConfirm,
        ConflictConfirm,
    };

    enum class FailReason {
        None,
        CannotOpenFile,
        ParseError,
        NeedRetry,
        NeedReload,
        SaveError,
    };

public:
    BundleImporter(
        const std::shared_ptr<BundleDepot>& depot,
        const std::filesystem::path& target_directory_path,
        const std::filesystem::path& bundle_path);

    void Import();
    void Cofirm();

private:
    bool ParseBundle();
    bool CheckIfCanSaveDirectly();
    bool SaveBundle();
    std::filesystem::path GetBundleSavePath() const;
    void ChangeToFailState(FailReason reason);

private:
    std::weak_ptr<BundleDepot> depot_;
    std::filesystem::path depot_directory_path_;
    std::filesystem::path bundle_path_;

    State state_{ State::Pending };
    FailReason fail_reason_{ FailReason::None };
    std::shared_ptr<Bundle> parsed_bundle_;
    std::vector<std::shared_ptr<Entry>> conflict_entries_;
};

}