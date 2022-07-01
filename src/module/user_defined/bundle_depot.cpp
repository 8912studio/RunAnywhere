#include "module/user_defined/bundle_depot.h"

namespace ra::module::user_defined {

void BundleDepot::Clear() {

    bundles_.clear();
}


void BundleDepot::AddBundle(const std::shared_ptr<UserDefinedBundle>& bundle) {

    bundles_.push_back(bundle);
}


std::shared_ptr<UserDefinedEntry> BundleDepot::FindEntry(std::wstring_view keyword) {

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (each_entry->Keyword() == keyword) {
                return each_entry;
            }
        }
    }

    return nullptr;
}


std::vector<std::shared_ptr<UserDefinedEntry>> BundleDepot::FindEntriesBeginWith(
    std::wstring_view keyword) {

    std::vector<std::shared_ptr<UserDefinedEntry>> result;

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (each_entry->Keyword().find(keyword) == 0) {
                result.push_back(each_entry);
            }
        }
    }

    return result;
}

}