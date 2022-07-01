#pragma once

#include <memory>
#include <vector>
#include "module/user_defined/user_defined_bundle.h"

namespace ra::module::user_defined {

class BundleDepot {
public:
    void Clear();

    void AddBundle(const std::shared_ptr<UserDefinedBundle>& bundle);

    std::shared_ptr<UserDefinedEntry> FindEntry(std::wstring_view keyword);
    std::vector<std::shared_ptr<UserDefinedEntry>> FindEntriesBeginWith(std::wstring_view keyword);

private:
    std::vector<std::shared_ptr<UserDefinedBundle>> bundles_;
};

}