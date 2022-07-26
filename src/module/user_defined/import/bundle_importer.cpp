#include "module/user_defined/import/bundle_importer.h"
#include <zaf/base/error/check.h>
#include "module/user_defined/bundle_definition.h"
#include "module/user_defined/bundle_parser.h"

namespace ra::module::user_defined {

BundleImporter::BundleImporter(
    const std::shared_ptr<BundleDepot>& depot,
    const std::filesystem::path& target_directory_path,
    const std::filesystem::path& bundle_path) 
    :
    depot_(depot),
    depot_directory_path_(target_directory_path),
    bundle_path_(bundle_path) {

}


void BundleImporter::Import() {

    ZAF_EXPECT(state_ == State::Pending);

    if (!ParseBundle()) {
        return;
    }

    if (!CheckIfCanSaveDirectly()) {
        return;
    }

    if (!SaveBundle()) {
        return;
    }

    state_ = State::Success;
}


bool BundleImporter::ParseBundle() {

    try {

        BundleParser parser(bundle_path_);
        parsed_bundle_ = parser.Parse();
        return true;
    }
    catch (const BundleParser::ParseError& error) {

        ChangeToFailState(FailReason::ParseError);
        parser_error_ = error;
        return false;
    }
    catch (const zaf::Error&) {

        ChangeToFailState(FailReason::CannotOpenFile);
        return false;
    }
}


bool BundleImporter::CheckIfCanSaveDirectly() {

    //The depot has changed, need to retry.
    auto depot = depot_.lock();
    if (!depot) {
        ChangeToFailState(FailReason::NeedRetry);
        return false;
    }

    auto bundle_save_path = GetBundleSavePath();

    std::error_code error_code;
    bool file_already_exist = std::filesystem::exists(bundle_save_path, error_code);

    auto found_bundle = depot->FindBundle(parsed_bundle_->Meta()->BundleID());

    //Loaded bundle and its file are inconsistent, a reload is needed.
    if (file_already_exist != !!found_bundle) {
        ChangeToFailState(FailReason::NeedReload);
        return false;
    }

    if (file_already_exist) {
        state_ = State::OverrideConfirm;
        return false;
    }

    conflict_entries_ = depot->FindConflictEntries(*parsed_bundle_);
    if (!conflict_entries_.empty()) {
        state_ = State::ConflictConfirm;
        return false;
    }

    return true;
}


bool BundleImporter::SaveBundle() {

    auto depot = depot_.lock();
    if (!depot) {
        ChangeToFailState(FailReason::NeedRetry);
        return false;
    }

    auto save_path = GetBundleSavePath();

    try {

        std::filesystem::create_directories(save_path.parent_path());

        std::filesystem::copy_file(
            bundle_path_,
            save_path,
            std::filesystem::copy_options::overwrite_existing);

        depot->AddOrReplaceBundle(parsed_bundle_);
        return true;
    }
    catch (const std::filesystem::filesystem_error& error) {

        ChangeToFailState(FailReason::SaveError);
        save_error_ = error.code();
        return false;
    }
}


void BundleImporter::Confirm() {

    ZAF_EXPECT(state_ == State::OverrideConfirm || state_ == State::ConflictConfirm);

    if (!SaveBundle()) {
        return;
    }

    state_ = State::Success;
}


std::filesystem::path BundleImporter::GetBundleSavePath() const {

    auto file_name = parsed_bundle_->Meta()->BundleID() + BundleFileExtension;
    return depot_directory_path_ / file_name;
}


void BundleImporter::ChangeToFailState(FailReason reason) {

    state_ = State::Fail;
    fail_reason_ = reason;
}

}