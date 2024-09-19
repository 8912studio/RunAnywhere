#pragma once

#include <string>

namespace ra::mod::ai::gpt {

constexpr const wchar_t* RoleSystem = L"system";
constexpr const wchar_t* RoleUser = L"user";
constexpr const wchar_t* RoleAssistant = L"assistant";

class Message {
public:
    explicit Message(std::wstring content) :
        role_(RoleUser),
        content_(std::move(content)) {

    }

    Message(std::wstring role, std::wstring content) : 
        role_(std::move(role)),
        content_(std::move(content)) {
    
    }

    const std::wstring& Role() const {
        return role_;
    }

    const std::wstring& Content() const {
        return content_;
    }

private:
    std::wstring role_;
    std::wstring content_;
};

}