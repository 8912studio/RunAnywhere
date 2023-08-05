#pragma once

#include <functional>
#include <string>

namespace ra::mod::meta {

class MetaCommandInfo {
public:
    std::wstring command;
    std::wstring description;
    std::function<void()> handler;
};

const std::vector<MetaCommandInfo>& GetMetaCommandInfos();
const MetaCommandInfo* GetMetaCommandInfo(const std::wstring& command);

}