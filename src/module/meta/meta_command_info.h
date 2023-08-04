#pragma once

#include <functional>
#include <string>
#include "module/command_preview_control.h"

namespace ra::mod::meta {

class MetaCommandInfo {
public:
    std::wstring command;
    std::wstring description;
    std::function<std::shared_ptr<CommandPreviewControl>()> preview_control_creator;
    std::function<void()> handler;
};

const std::vector<MetaCommandInfo>& GetMetaCommandInfos();
const MetaCommandInfo* GetMetaCommandInfo(const std::wstring& command);

}