#include "option_storage.h"
#include <zaf/base/error/error.h>
#include <zaf/base/registry/registry.h>
#include "registry_define.h"

namespace ra {
namespace {

constexpr const wchar_t* AutoHideValueName = L"AutoHideOnLostFocus";
constexpr const wchar_t* RememberLastCommandValueName = L"RememberLastCommand";
constexpr const wchar_t* MaxPreservedCommandCountValueName = L"MaxPreservedCommandCount";

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


std::optional<std::size_t> OptionStorage::GetUIntValue(const std::wstring& name) {

    try {
        return option_key_.GetDWordValue(name);
    }
    catch (const zaf::Error&) {
        return std::nullopt;
    }
}


void OptionStorage::SetUIntValue(const std::wstring& name, std::size_t value) {

    try {
        option_key_.SetDWordValue(name, static_cast<std::uint32_t>(value));
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


bool OptionStorage::RememberLastCommand() {

    if (!remember_last_command_) {
        remember_last_command_ = GetBoolValue(RememberLastCommandValueName).value_or(false);
    }
    return *remember_last_command_;
}


void OptionStorage::SetRememberLastCommand(bool value) {

    remember_last_command_ = value;
    SetBoolValue(RememberLastCommandValueName, value);
}


std::size_t OptionStorage::MaxPreservedCommandCount() {

    if (!max_preserved_command_count_) {
        max_preserved_command_count_ = GetUIntValue(MaxPreservedCommandCountValueName).value_or(1);
    }
    return *max_preserved_command_count_;
}

void OptionStorage::SetMaxPreservedCommandCount(std::size_t count) {

    auto new_count = count;
    if (new_count < 1) {
        new_count = 1;
    }
    else if (new_count > 10) {
        new_count = 10;
    }

    *max_preserved_command_count_ = new_count;
    SetUIntValue(MaxPreservedCommandCountValueName, new_count);
}

}