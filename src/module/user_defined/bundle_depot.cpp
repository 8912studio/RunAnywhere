#include "module/user_defined/bundle_depot.h"
#include <set>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/case_conversion.h>

namespace ra::module::user_defined {

void BundleDepot::AddBundle(const std::shared_ptr<Bundle>& bundle) {

    bundles_.push_back(bundle);
}


void BundleDepot::AddOrReplaceBundle(const std::shared_ptr<Bundle>& bundle) {

    auto checked_bundle_id = zaf::ToLowercased(bundle->Meta()->BundleID());

    for (auto iterator = bundles_.begin(); iterator != bundles_.end(); ++iterator) {

        auto lower_bundle_id = zaf::ToLowercased((*iterator)->Meta()->BundleID());
        if (lower_bundle_id == checked_bundle_id) {

            *iterator = bundle;
            return;
        }
    }

    AddBundle(bundle);
}


std::shared_ptr<Bundle> BundleDepot::FindBundle(const std::wstring& bundle_id) {

    auto checked_bundle_id = zaf::ToLowercased(bundle_id);

    for (const auto& each_bundle : bundles_) {

        auto lower_bundle_id = zaf::ToLowercased(each_bundle->Meta()->BundleID());
        if (lower_bundle_id == checked_bundle_id) {
            return each_bundle;
        }
    }

    return nullptr;
}


std::shared_ptr<Entry> BundleDepot::FindEntry(const std::wstring& keyword) {

    auto checked_keyword = zaf::ToLowercased(keyword);

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            auto lower_keyword = zaf::ToLowercased(each_entry->Keyword());
            if (lower_keyword == checked_keyword) {
                return each_entry;
            }
        }
    }

    return nullptr;
}


std::vector<std::shared_ptr<Entry>> BundleDepot::FindEntriesBeginWith(
    const std::wstring& keyword) {

    auto checked_keyword = zaf::ToLowercased(keyword);

    std::vector<std::shared_ptr<Entry>> result;

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            auto lower_keyword = zaf::ToLowercased(each_entry->Keyword());
            if (lower_keyword.find(checked_keyword) == 0) {
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
        checked_keywords.insert(zaf::ToLowercased(each_entry->Keyword()));
    }

    std::vector<std::shared_ptr<Entry>> result;

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            auto lower_keyword = zaf::ToLowercased(each_entry->Keyword());
            if (zaf::Contain(checked_keywords, lower_keyword)) {
                result.push_back(each_entry);
            }
        }
    }

    return result;
}

}