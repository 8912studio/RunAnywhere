#pragma once

#include <filesystem>
#include "module/user_defined/user_defined_bundle.h"

namespace ra::module::user_defined {

class UserDefinedBundleParser {
public:
    UserDefinedBundleParser(const std::filesystem::path& bundle_path);

    UserDefinedBundleParser(const UserDefinedBundleParser&) = delete;
    UserDefinedBundleParser& operator=(const UserDefinedBundleParser&) = delete;

    std::shared_ptr<UserDefinedBundle> Parse();

private:
    std::filesystem::path bundle_path_;
};

}