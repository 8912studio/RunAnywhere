#pragma once

#include <filesystem>
#include <optional>
#include <zaf/base/non_copyable.h>
#include "module/user_defined/bundle_depot.h"
#include "module/user_defined/bundle_parser.h"

namespace ra::mod::user_defined {

class BundleImporter : zaf::NonCopyableNonMovable {
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
    void Confirm();

    const std::filesystem::path& GetBundlePath() const {
        return bundle_path_;
    }

    State GetState() const {
        return state_;
    }

    FailReason GetFailReason() const {
        return fail_reason_;
    }

    const std::vector<std::shared_ptr<Entry>>& GetConflictEntries() const {
        return conflict_entries_;
    }

    const BundleParser::ParseError* GetParseError() const {
        return parser_error_ ? &parser_error_.value() : nullptr;
    }

    const std::error_code* GetSaveError() const {
        return save_error_ ? &save_error_.value() : nullptr;
    }

private:
    bool ParseBundle();
    bool CheckIfCanSaveDirectly();
    bool AreAllBundlFileExist(const std::vector<std::shared_ptr<Entry>>& entries) const;
    bool SaveBundle();
    std::filesystem::path GetBundleSavePath() const;
    std::filesystem::path GetBundlePathInDepot(const std::wstring& bundle_id) const;
    void ChangeToFailState(FailReason reason);

private:
    std::weak_ptr<BundleDepot> depot_;
    std::filesystem::path depot_directory_path_;
    std::filesystem::path bundle_path_;

    State state_{ State::Pending };
    FailReason fail_reason_{ FailReason::None };
    std::shared_ptr<Bundle> parsed_bundle_;
    std::vector<std::shared_ptr<Entry>> conflict_entries_;
    std::optional<BundleParser::ParseError> parser_error_;
    std::optional<std::error_code> save_error_;
};

}