#pragma once

#include <optional>
#include <zaf/base/error/check.h>
#include <zaf/base/non_copyable.h>
#include <zaf/base/registry/registry_key.h>
#include "module/user_defined/bundle_meta.h"

namespace ra::module::user_defined {

class VariableFormatter : zaf::NonCopyable {
public:
    VariableFormatter(const std::shared_ptr<BundleMeta>& bundle_meta) : bundle_meta_(bundle_meta) {
        ZAF_EXPECT(bundle_meta_);
    }

    std::wstring Format(std::wstring_view input);

private:
    class VariableModifier {
    public:
        bool do_not_expand{};
        bool do_not_check_existent{};
    };

private:
    static std::optional<VariableModifier> ParseVariableModifier(std::wstring_view modifier);
    static std::optional<std::wstring> ExpandVariableContent(
        const std::wstring& content,
        const VariableModifier& modifier);
    static std::optional<std::wstring> TryToExpandRegistryContent(const std::wstring& content);

private:
    std::optional<std::wstring> FormatVariable(std::wstring_view input, std::size_t& index);
    std::wstring GetVariableContent(std::wstring_view name, const VariableModifier& modifier);

private:
    std::shared_ptr<BundleMeta> bundle_meta_;
};

}