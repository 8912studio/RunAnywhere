#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <zaf/base/non_copyable.h>

namespace ra::utility {

class LogManager : public zaf::NonCopyableNonMovable {
public:
    static LogManager& Instance();

    void Initialize();
    void Uninitialize() noexcept;

    void WriteLine(std::string_view line);

private:
    LogManager() = default;

    std::filesystem::path GetLogDirectoryPath() const;

private:
    std::mutex log_file_lock_;
    std::ofstream log_file_;
};

}