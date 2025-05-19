#include "utility/log/log_manager.h"
#include <chrono>
#include <filesystem>
#include <zaf/base/string/encoding_conversion.h>
#include "utility/app_directory.h"

namespace ra::utility {

LogManager& LogManager::Instance() {
    static LogManager instance;
    return instance;
}


void LogManager::Initialize() {

    auto log_directory_path = GetLogDirectoryPath();
    std::filesystem::create_directories(log_directory_path);

    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    std::chrono::zoned_time current_time{ std::chrono::current_zone(), now_seconds };
    auto file_name = std::format("{:%Y%m%d-%H%M%S}.log", current_time);

    auto log_file_path = log_directory_path / file_name;
    log_file_.open(log_file_path, std::ios::out | std::ios::app);
}


void LogManager::Uninitialize() noexcept {

    log_file_.close();

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


void LogManager::WriteLine(std::string_view line) {

    std::scoped_lock<std::mutex> lock_guard(log_file_lock_);

    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    std::chrono::zoned_time current_time{ std::chrono::current_zone(), now_ms };

    std::ostringstream stream;
    stream << std::format("{:%H:%M:%S}", current_time) << ' '
        << GetCurrentThreadId() << ' '
        << line << "\n";

    log_file_ << stream.view();
    log_file_.flush();

#ifndef NDEBUG
    OutputDebugString(zaf::FromUTF8String(stream.view()).c_str());
#endif
}

}
