#include "module/user_defined/bundle_importer.h"
#include <zaf/base/error/check.h>
#include "module/user_defined/bundle_parser.h"

namespace ra::module::user_defined {

BundleImporter::BundleImporter(
    const std::shared_ptr<BundleDepot>& depot,
    const std::filesystem::path& target_directory_path,
    const std::filesystem::path& bundle_path) 
    :
    depot_(depot),
    target_directory_path_(target_directory_path),
    bundle_path_(bundle_path) {

}


void BundleImporter::Import() {

    ZAF_EXPECT(state_ == State::Pending);

    if (!ParseBundle()) {
        return;
    }

    auto depot = depot_.lock();
    if (!depot) {
        return;
    }

    auto found_bundle = depot->FindBundle(parsed_bundle_->Meta()->BundleID());
    if (found_bundle) {
        return;
    }

    
}


bool BundleImporter::ParseBundle() {

    try {

        BundleParser parser(bundle_path_);
        parsed_bundle_ = parser.Parse();
        return true;
    }
    catch (const zaf::Error& error) {

        return false;
    }
}


void BundleImporter::SaveBundle() {


}

}