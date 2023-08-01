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

    bool RememberLastCommand();
    void SetRememberLastCommand(bool value);

    std::size_t MaxPreservedCommandCount();
    void SetMaxPreservedCommandCount(std::size_t count);

private:
    OptionStorage();

    std::optional<bool> GetBoolValue(const std::wstring& name);
    void SetBoolValue(const std::wstring& name, bool value);

    std::optional<std::size_t> GetUIntValue(const std::wstring& name);
    void SetUIntValue(const std::wstring& name, std::size_t value);

private:
    zaf::RegistryKey option_key_;

    std::optional<bool> auto_hide_;
    std::optional<bool> remember_last_command_;
    std::optional<std::size_t> max_preserved_command_count_;
};

}