#include "context/discover/vscode_discoverer.h"
#include <fstream>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/handle.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/to_numeric.h>
#include "context/discover/window_based_discoverer.h"
#include "utility/data_directory.h"

namespace ra::context {

VSCodeDiscoverer::VSCodeDiscoverer() {

    auto ipc_directory_path = utility::GetDataDirectoryPath() / "VSCodeIPC";

    request_directory_path_ = ipc_directory_path / "Request";
    response_directory_path_ = request_directory_path_ / "Response";
}


ActivePath VSCodeDiscoverer::Discover(const ForegroundWindowInfo& foreground_window_info) {

    if (!IsVSCodeProcess(foreground_window_info.process_id)) {
        return {};
    }

    if (!CreateRequestFile()) {
        return {};
    }

    if (!WaitForResponse(response_directory_path_)) {
        return {};
    }

    return ReadResponseFile(response_directory_path_, sequence_);
}


bool VSCodeDiscoverer::IsVSCodeProcess(DWORD process_id) {

    zaf::Handle process_handle{ OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id) };
    if (!process_handle.IsValid()) {
        return false;
    }

    wchar_t path_buffer[MAX_PATH]{};
    DWORD path_buffer_size{ MAX_PATH };
    BOOL is_succeeded = QueryFullProcessImageName(
        process_handle.Value(),
        0,
        path_buffer,
        &path_buffer_size);

    if (!is_succeeded) {
        return false;
    }

    //Just check the file name only for now. Maybe add more detail to check in the future.
    auto filename = zaf::ToLowercased(std::filesystem::path{ path_buffer }.filename());
    return filename == L"code.exe";
}


bool VSCodeDiscoverer::CreateRequestFile() {

    try {
        std::filesystem::create_directories(request_directory_path_);
        std::filesystem::create_directories(response_directory_path_);
    }
    catch (const std::filesystem::filesystem_error&) {
        return false;
    }

    constexpr std::size_t max_try_count{ 100 };
    for (auto _ : zaf::Range(max_try_count)) {

        ++sequence_;

        auto request_file_path = TryToCreateRequestFile(request_directory_path_, sequence_);
        if (!request_file_path.empty()) {
            requesting_file_path_ = request_file_path;
            return true;
        }
    }

    return false;
}


std::filesystem::path VSCodeDiscoverer::TryToCreateRequestFile(
    const std::filesystem::path& directory_path, 
    std::uint32_t sequence) {

    try {

        auto request_file_path = directory_path / ("request-" + std::to_string(sequence));

        if (std::filesystem::exists(request_file_path)) {
            std::filesystem::remove(request_file_path);
        }

        std::ofstream file_stream(request_file_path, std::ofstream::out | std::ofstream::binary);
        if (!file_stream.is_open()) {
            return {};
        }

        return request_file_path;
    }
    catch (const std::filesystem::filesystem_error&) {
        return {};
    }
}


bool VSCodeDiscoverer::WaitForResponse(const std::filesystem::path& directory_path) {

    HANDLE handle = FindFirstChangeNotification(
        directory_path.c_str(),
        FALSE,
        FILE_NOTIFY_CHANGE_FILE_NAME);

    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD wait_result = WaitForSingleObject(handle, 2000);
    FindCloseChangeNotification(handle);
    return wait_result == WAIT_OBJECT_0;
}


ActivePath VSCodeDiscoverer::ReadResponseFile(
    const std::filesystem::path& directory_path, 
    std::uint32_t sequence) {

    try {

        const std::wstring response_prefix{ L"response-" };
        ActivePath result;

        for (std::filesystem::directory_iterator iterator(directory_path); 
             iterator != std::filesystem::directory_iterator(); 
             ++iterator) {

            auto filename = iterator->path().filename().wstring();
            if (filename.find(response_prefix) == 0) {

                auto responsed_sequence = zaf::ToNumeric<std::uint32_t>(
                    filename.substr(response_prefix.length()));

                if (responsed_sequence == sequence) {
                    result = ReadActivePathFromFile(iterator->path());
                }
            }

            std::error_code error;
            std::filesystem::remove(iterator->path(), error);
        }

        return result;
    }
    catch (const std::filesystem::filesystem_error&) {
        return {};
    }
}


ActivePath VSCodeDiscoverer::ReadActivePathFromFile(const std::filesystem::path& file_path) {

    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        file_stream.open(file_path, std::ifstream::in | std::ifstream::binary);

        file_stream.seekg(0, std::ifstream::end);
        auto file_size = file_stream.tellg();

        auto buffer_size = std::min(file_size, static_cast<std::streampos>(4096));
        std::string buffer(static_cast<std::size_t>(buffer_size), 0);

        file_stream.read(&buffer[0], buffer_size);

        return WindowBasedDiscoverer::DecodeActivePath(zaf::FromUtf8String(buffer));
    }
    catch (const std::ifstream::failure&) {
        return {};
    }
}

}