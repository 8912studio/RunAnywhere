#include "option_storage.h"
#include <zaf/base/error/error.h>
#include <zaf/base/registry/registry.h>
#include "registry_define.h"

namespace ra {
namespace {

constexpr const wchar_t* const AutoHideValueName = L"AutoHideOnLostFocus";

}

OptionStorage& OptionStorage::Instance() {
    static OptionStorage instance;
    return instance;
}


OptionStorage::OptionStorage() {

    try {

        option_key_ = zaf::Registry::CurrentUser().OpenSubKey(
            RegistrySubKeyPath,
            zaf::RegistryRights::Read | zaf::RegistryRights::Write);
    }
    catch (const zaf::Error&) {

    }
}


std::optional<bool> OptionStorage::GetBoolValue(const std::wstring& name) {

    try {
        auto value = option_key_.GetDWordValue(name);
        return !!value;
    }
    catch (const zaf::Error&) {
        return std::nullopt;
    }
}


void OptionStorage::SetBoolValue(const std::wstring& name, bool value) {

    try {
        option_key_.SetDWordValue(name, value);
    }
    catch (const zaf::Error&) {

    }
}


bool OptionStorage::AutoHideOnLostFocus() {

    if (!auto_hide_) {
        auto_hide_ = GetBoolValue(AutoHideValueName).value_or(true);
    }
    return *auto_hide_;
}


void OptionStorage::SetAutoHideOnLostFocus(bool value) {

    auto_hide_ = value;
    SetBoolValue(AutoHideValueName, value);
}

}