#pragma once

#include <filesystem>
#include <zaf/base/non_copyable.h>

namespace ra::utility {

class LogManager : public zaf::NonCopyableNonMovable {
public:
    static LogManager& Instance();

    void Initialize();
    void Uninitialize() noexcept;

private:
    LogManager() = default;

    std::filesystem::path GetLogDirectoryPath() const;
};

}