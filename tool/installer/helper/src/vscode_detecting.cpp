#include "vscode_detecting.h"
#include <zaf/base/error/error.h>
#include <zaf/base/registry/registry.h>

namespace {

std::wstring Detect() {

    try {

        auto command = zaf::Registry::ClassesRoot().GetStringValue(
            L"Applications\\Code.exe\\Shell\\Open\\Command",
            std::wstring{});

        int argument_count{};
        auto arguments = CommandLineToArgvW(command.c_str(), &argument_count);
        if (!arguments) {
            return std::wstring{};
        }

        std::wstring result;
        if (argument_count > 1) {
            result = arguments[0];
        }

        LocalFree(arguments);
        return result;
    }
    catch (const zaf::Error&) {
        return std::wstring{};
    }
}

}


std::wstring GetVSCodeExePath() {

    static const auto result = Detect();
    return result;
}