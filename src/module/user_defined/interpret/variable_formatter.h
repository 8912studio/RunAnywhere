#pragma once

#include <optional>
#include <zaf/base/non_copyable.h>
#include "module/user_defined/bundle_meta.h"

namespace ra::module::user_defined {

class VariableFormatter : zaf::NonCopyable {
public:
    VariableFormatter(const std::shared_ptr<BundleMeta>& bundle_meta) : bundle_meta_(bundle_meta) {

    }

    std::wstring Format(std::wstring_view input);

private:
    std::optional<std::wstring> FormatVariable(std::wstring_view input, std::size_t& index);

private:
    std::shared_ptr<BundleMeta> bundle_meta_;
};

}