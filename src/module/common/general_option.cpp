#include "module/common/general_option.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::mod {
namespace {

bool ParseSwitch(std::wstring_view text, GeneralOption& option) {

    if (text == L"f") {
        option.treat_text_as_file = true;
        return true;
    }
    
    if (text == L"u8") {
        option.text_encoding = TextEncoding::UTF8;
        return true;
    }
    
    if (text == L"u16") {
        option.text_encoding = TextEncoding::UTF16;
        return true;
    }

    return false;
}


bool ParseNormalText(std::wstring_view text, GeneralOption& option) {

    auto active_path_option = active_path::TryToParseActivePathArgument(text);
    if (active_path_option) {
        option.active_path_option = active_path_option;
        return true;
    }

    if (text.front() == L'/') {
        return ParseSwitch(text.substr(1), option);
    }

    option.text = text;
    return true;
}

}

bool ParseGeneralOption(const utility::CommandLinePiece& argument, GeneralOption& option) {

    if (argument.Type() == utility::CommandLinePieceType::NormalText) {
        return ParseNormalText(argument.Content(), option);
    }
    else if (argument.Type() == utility::CommandLinePieceType::TextBlock) {
        option.text = argument.Content();
        return true;
    }
    return false;
}

}