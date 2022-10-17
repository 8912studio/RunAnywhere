#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class VSCodeDiscoverer : public Discoverer {
public:
    VSCodeDiscoverer();

    ActivePath Discover(const ForegroundWindowInfo& foreground_window_info) override;

private:
    static bool IsVSCodeProcess(DWORD process_id);

private:
    std::filesystem::path request_directory_path_;
    std::filesystem::path response_directory_path_;
    std::uint32_t sequence_{};
    std::filesystem::path requesting_file_path_;
};

}