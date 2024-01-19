#pragma once

#include <string>
#include <vector>
#include <run_anywhere_extension_sdk.h>

class ExtensionCommand {
public:
    bool Interpret(const RA_CommandLine* command_line);

    const std::wstring& GetText() const {
        return text_;
    }

private:
    std::wstring text_;
};