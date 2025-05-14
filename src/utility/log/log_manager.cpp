#include "utility/log/log_manager.h"
#include <filesystem>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include "utility/app_directory.h"

namespace ra::utility {

LogManager& LogManager::Instance() {
    static LogManager instance;
    return instance;
}


void LogManager::Initialize() {

    auto log_directory_path = GetLogDirectoryPath();
    std::filesystem::create_directories(log_directory_path);

    boost::log::add_common_attributes();

    boost::log::add_file_log(
        boost::log::keywords::file_name = log_directory_path / L"%Y%m%d-%H%M%S.log",
        boost::log::keywords::format = "[%TimeStamp%][%ThreadID%] %Message%",
        boost::log::keywords::auto_flush = true
    );

    //We do not use file collector for auto-deleting old log files, because it needs to scan all 
    //files in the directory, which may be costly and slow down the application startup. Instead, 
    //we manually delete old log files on the application termination.
}


void LogManager::Uninitialize() noexcept {

    //Remove old log files.
    try {

        auto log_directory_path = GetLogDirectoryPath();

        auto now = std::filesystem::file_time_type::clock::now();

        for (const auto& each_file : std::filesystem::directory_iterator(log_directory_path)) {

            if (!each_file.is_regular_file()) {
                continue;
            }

            if (each_file.path().extension() != L".log") {
                continue;
            }

            auto last_write_time = std::filesystem::last_write_time(each_file.path());
            if (now - last_write_time <= std::chrono::days(5)) {
                continue;
            }

            //Pass error code to skip files that cannot be removed.
            std::error_code error_code;
            std::filesystem::remove(each_file.path(), error_code);
        }
    }
    catch (...) {

    }
}


std::filesystem::path LogManager::GetLogDirectoryPath() const {
    return GetAppDataDirectoryPath() / L"Log";
}

}
