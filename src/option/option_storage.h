#pragma once

#include <optional>
#include <zaf/base/registry/registry_key.h>

namespace ra::option {

class OptionStorage {
public:
    static OptionStorage& Instance();

    static constexpr std::size_t MinMaxPreservedCommandCount = 1;
    static constexpr std::size_t MaxMaxPreservedCommandCount = 10;

    static constexpr const wchar_t* DefaultOpenAIAPIServer = L"https://api.openai.com";

public:
    OptionStorage(const OptionStorage&) = delete;
    OptionStorage& operator=(const OptionStorage&) = delete;

    bool AutoHideOnLostFocus();
    void SetAutoHideOnLostFocus(bool value);

    bool RememberLastCommand();
    void SetRememberLastCommand(bool value);

    std::size_t MaxPreservedCommandCount();
    void SetMaxPreservedCommandCount(std::size_t count);

    std::wstring OpenAIAPIServer();
    void SetOpenAIAPIServer(const std::wstring& value);

    std::wstring OpenAIAPIKey();
    void SetOpenAIAPIKey(const std::wstring& key);

    std::wstring Proxy();
    void SetProxy(const std::wstring& proxy);

private:
    OptionStorage();

    std::optional<bool> GetBoolValue(const std::wstring& name) const;
    void SetBoolValue(const std::wstring& name, bool value);

    std::optional<std::size_t> GetUIntValue(const std::wstring& name) const;
    void SetUIntValue(const std::wstring& name, std::size_t value);

    std::optional<std::wstring> GetStringValue(const std::wstring& name) const;
    void SetStringValue(const std::wstring& name, const std::wstring& value);

private:
    zaf::RegistryKey option_key_;

    std::optional<bool> auto_hide_;
    std::optional<bool> remember_last_command_;
    std::optional<std::size_t> max_preserved_command_count_;
    std::optional<std::wstring> open_ai_api_server_;
    std::optional<std::wstring> open_ai_api_key_;
    std::optional<std::wstring> proxy_;
};

}