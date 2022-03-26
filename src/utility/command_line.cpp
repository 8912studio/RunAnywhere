#include "utility/command_line.h"
#include <Windows.h>
#include <zaf/base/container/utility/range.h>

namespace ra::utility {

CommandLine::CommandLine(const std::wstring& text) : text_(text) {

    int count{};
    auto parts = CommandLineToArgvW(text.c_str(), &count);
    if (!parts) {
        return;
    }

    for (auto index : zaf::Range(0, count)) {

        all_parts_.push_back(parts[index]);

        if (index == 0) {
            command_ = parts[index];
        }
        else {
            arguments_.push_back(parts[index]);
        }
    }

    LocalFree(parts);
}

}