#pragma once

#include <optional>
#include <zaf/base/registry/registry_key.h>

namespace ra {

class OptionStorage {
public:
    static OptionStorage& Instance();

public:
    OptionStorage(const OptionStorage&) = delete;
    OptionStorage& operator=(const OptionStorage&) = delete;

    bool AutoHideOnLostFocus();
    void SetAutoHideOnLostFocus(bool value);

private:
    OptionStorage();

    std::optional<bool> GetBoolValue(const std::wstring& name);
    void SetBoolValue(const std::wstring& name, bool value);

private:
    zaf::RegistryKey option_key_;

    std::optional<bool> auto_hide_;
};

}