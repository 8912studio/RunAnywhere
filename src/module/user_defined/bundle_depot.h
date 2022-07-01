#pragma once

#include <memory>
#include <vector>
#include "module/user_defined/bundle.h"

namespace ra::module::user_defined {

class BundleDepot {
public:
    void AddBundle(const std::shared_ptr<Bundle>& bundle);

    std::shared_ptr<Bundle> FindBundle(const std::wstring& bundle_id);

    std::shared_ptr<Entry> FindEntry(std::wstring_view keyword);
    std::vector<std::shared_ptr<Entry>> FindEntriesBeginWith(std::wstring_view keyword);

private:
    std::vector<std::shared_ptr<Bundle>> bundles_;
};

}