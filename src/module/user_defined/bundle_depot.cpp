#include "module/user_defined/bundle_depot.h"
#include <set>
#include <zaf/base/container/utility/contain.h>

namespace ra::module::user_defined {

void BundleDepot::AddBundle(const std::shared_ptr<Bundle>& bundle) {

    bundles_.push_back(bundle);
}


std::shared_ptr<Bundle> BundleDepot::FindBundle(const std::wstring& bundle_id) {

    for (const auto& each_bundle : bundles_) {

        if (each_bundle->Meta()->BundleID() == bundle_id) {
            return each_bundle;
        }
    }

    return nullptr;
}


std::shared_ptr<Entry> BundleDepot::FindEntry(std::wstring_view keyword) {

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (each_entry->Keyword() == keyword) {
                return each_entry;
            }
        }
    }

    return nullptr;
}


std::vector<std::shared_ptr<Entry>> BundleDepot::FindEntriesBeginWith(
    std::wstring_view keyword) {

    std::vector<std::shared_ptr<Entry>> result;

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (each_entry->Keyword().find(keyword) == 0) {
                result.push_back(each_entry);
            }
        }
    }

    return result;
}


std::vector<std::shared_ptr<Entry>> BundleDepot::FindConflictEntries(
    const Bundle& bundle) {

    std::set<std::wstring> checked_keywords;
    for (const auto& each_entry : bundle.Entries()) {
        checked_keywords.insert(each_entry->Keyword());
    }

    std::vector<std::shared_ptr<Entry>> result;

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (zaf::Contain(checked_keywords, each_entry->Keyword())) {
                result.push_back(each_entry);
            }
        }
    }

    return result;
}

}