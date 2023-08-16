#pragma once

#include <filesystem>
#include <string>
#include <variant>
#include "context/desktop_context.h"
#include "module/common/general_option.h"
#include "module/common/text_encoding.h"

namespace ra::mod {

class GeneralInput {
public:
    class Text {
    public:
        std::wstring content;
        TextEncoding encoding{ TextEncoding::UTF8 };
    };

public:
    GeneralInput(std::wstring text, TextEncoding encoding) : 
        data_(Text(std::move(text), encoding)) {

    }

    explicit GeneralInput(std::filesystem::path file_path) : data_(std::move(file_path)) {

    }

    const Text* GetText() const {
        return std::get_if<Text>(&data_);
    }

    const std::filesystem::path* GetFile() const {
        return std::get_if<std::filesystem::path>(&data_);
    }

private:
    std::variant<Text, std::filesystem::path> data_;
};


GeneralInput MakeGeneralInput(const context::DesktopContext& context, const GeneralOption& option);

}