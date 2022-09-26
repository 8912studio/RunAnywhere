#pragma once

#include <string>

class NPPDetectResult {
public:
    std::wstring path;
    bool is_x86{};
};

NPPDetectResult DetectNPPInstalledDirectoryPath();