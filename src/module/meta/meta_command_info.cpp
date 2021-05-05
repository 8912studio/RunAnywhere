#include "module/meta/meta_command_info.h"
#include <zaf/base/container/utility/sort.h>
#include "about_window.h"
#include "main_window.h"
#include "option_window.h"

namespace ra::module::meta {
namespace {

std::vector<MetaCommandInfo> CreateMetaCommandInfos() {

    std::vector<MetaCommandInfo> result{
        {
            L"reload",
            L"Reload user-defined commands",
            []() {
                MainWindow::Instance().ReloadUserDefinedCommands();
            }
        },
        {
            L"about",
            L"Show about window",
            []() {
                AboutWindow::ShowInstance();
            }
        },
        {
            L"option",
            L"Show option window",
            []() {
                OptionWindow::ShowInstance();
            }
        }
    };

    zaf::Sort(result, [](const auto& info1, const auto& info2) {
        return info1.command < info2.command;
    });

    return result;
}


const std::vector<MetaCommandInfo>& GetMetaCommandInfos() {

    static std::vector<MetaCommandInfo> infos = CreateMetaCommandInfos();
    return infos;
}

}


const MetaCommandInfo* GetMetaCommand(const std::wstring& command) {

    const auto& command_infos = GetMetaCommandInfos();

    auto iterator = std::lower_bound(
        command_infos.begin(), 
        command_infos.end(),
        command, 
        [](const MetaCommandInfo& command_info, const std::wstring& command) {
    
        return command_info.command < command;
    });

    if (iterator == command_infos.end()) {
        return nullptr;
    }

    if (iterator->command != command) {
        return nullptr;
    }

    return &*iterator;
}

}