#pragma once

#include <optional>
#include <zaf/base/error/check.h>
#include <zaf/base/non_copyable.h>
#include <zaf/base/registry/registry_key.h>
#include "context/active_path.h"
#include "module/user_defined/bundle_meta.h"

namespace ra::module::user_defined {

class VariableFormatter : zaf::NonCopyable {
public:
    VariableFormatter(
        const std::shared_ptr<BundleMeta>& bundle_meta,
        const context::ActivePath& active_path);

    std::wstring Format(std::wstring_view input) const;

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
    std::optional<std::wstring> FormatVariable(std::wstring_view input, std::size_t& index) const;
    std::optional<std::wstring> FormatActivePathVariable(std::wstring_view variable) const;
    std::optional<std::wstring> FormatGeneralVariable(std::wstring_view variable) const;
    std::wstring GetVariableContent(std::wstring_view name, const VariableModifier& modifier) const;

private:
    std::shared_ptr<BundleMeta> bundle_meta_;
    context::ActivePath active_path_;
};

}