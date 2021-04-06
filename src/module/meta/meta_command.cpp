#include "module/meta/meta_command.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/creation.h>
#include "about_window.h"
#include "option_window.h"

namespace {

constexpr const wchar_t* const ShowAboutCommand = L"about";
constexpr const wchar_t* const ShowOptionCommand = L"option";

}

MetaCommand::MetaCommand(const std::wstring& command_text) : 
    command_text_(zaf::ToLowercased(command_text)) {

}


std::wstring MetaCommand::GetPreviewText() {

    constexpr const struct {
        const wchar_t* command;
        const wchar_t* text;
    } map[] = {
    
        { ShowAboutCommand, L"Show about window" },
        { ShowOptionCommand, L"Show option window" },
    };

    for (const auto& each_item : map) {

        if (each_item.command == command_text_) {
            return each_item.text;
        }
    }

    return std::wstring{};
}


void MetaCommand::Execute() {

    if (command_text_ == ShowAboutCommand) {
        AboutWindow::ShowInstance();
    }
    else if (command_text_ == ShowOptionCommand) {
        OptionWindow::ShowInstance();
    }
}