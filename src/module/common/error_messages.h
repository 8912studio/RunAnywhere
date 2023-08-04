#pragma once

namespace ra::mod {

class ErrorMessages {
public:
    static constexpr const wchar_t* NoContentToDisplay = L"No content to display";
    static constexpr const wchar_t* UnableToReadFile = L"Unable to read file";

public:
    ErrorMessages() = delete;
};

}