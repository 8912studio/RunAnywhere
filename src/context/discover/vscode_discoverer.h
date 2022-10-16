#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class VSCodeDiscoverer : public Discoverer {
public:
    VSCodeDiscoverer();

    ActivePath Discover(const ForegroundWindowInfo& foreground_window_info) override;

private:
    static bool IsVSCodeProcess(DWORD process_id);
    static std::filesystem::path TryToCreateRequestFile(
        const std::filesystem::path& directory_path, 
        std::uint32_t sequence);
    static bool WaitForResponse(const std::filesystem::path& directory_path);
    static ActivePath ReadResponseFile(
        const std::filesystem::path& directory_path, 
        std::uint32_t sequence);
    static ActivePath ReadActivePathFromFile(const std::filesystem::path& file_path);

private:
    bool CreateRequestFile();

private:
    std::filesystem::path request_directory_path_;
    std::filesystem::path response_directory_path_;
    std::uint32_t sequence_{};
    std::filesystem::path requesting_file_path_;
};

}