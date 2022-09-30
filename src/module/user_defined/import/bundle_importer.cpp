#include "module/user_defined/import/bundle_importer.h"
#include <set>
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

        //Conflict entries have found, make sure the corresponding bundle files exit, otherwise 
        //a reload is needed.
        if (!AreAllBundlFileExist(conflict_entries_)) {
            ChangeToFailState(FailReason::NeedReload);
            return false;
        }

        state_ = State::ConflictConfirm;
        return false;
    }

    return true;
}


bool BundleImporter::AreAllBundlFileExist(
    const std::vector<std::shared_ptr<Entry>>& entries) const {

    std::set<std::wstring> bundle_ids;
    for (const auto& each_entry : entries) {
        bundle_ids.insert(each_entry->BundleMeta()->BundleID());
    }

    try {

        for (const auto& each_id : bundle_ids) {

            auto bundle_path = GetBundlePathInDepot(each_id);
            if (!std::filesystem::exists(bundle_path)) {
                return false;
            }
        }

        return true;
    }
    catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}


bool BundleImporter::SaveBundle() {

    auto depot = depot_.lock();
    if (!depot) {
        ChangeToFailState(FailReason::NeedRetry);
        return false;
    }

    auto save_path = GetBundleSavePath();

    try {

        //The imported file may just locate in the depot directory.
        if (bundle_path_ != save_path) {

            std::filesystem::create_directories(save_path.parent_path());

            std::filesystem::copy_file(
                bundle_path_,
                save_path,
                std::filesystem::copy_options::overwrite_existing);
        }

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

    return GetBundlePathInDepot(parsed_bundle_->Meta()->BundleID());
}


std::filesystem::path BundleImporter::GetBundlePathInDepot(const std::wstring& bundle_id) const {

    auto file_name = bundle_id + BundleFileExtension;
    return depot_directory_path_ / file_name;
}


void BundleImporter::ChangeToFailState(FailReason reason) {

    state_ = State::Fail;
    fail_reason_ = reason;
}

}