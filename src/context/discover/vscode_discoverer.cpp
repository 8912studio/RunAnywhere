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
namespace {

constexpr const wchar_t* PipeName = LR"(\\.\pipe\Zplutor.RunAnywhere.Discover.VSCode)";

}


VSCodeDiscoverer::VSCodeDiscoverer() {

    auto ipc_directory_path = utility::GetDataDirectoryPath() / "VSCodeIPC";

    request_directory_path_ = ipc_directory_path / "Request";
    response_directory_path_ = request_directory_path_ / "Response";
}


ActivePath VSCodeDiscoverer::Discover(const ForegroundWindowInfo& foreground_window_info) {

    if (!IsVSCodeProcess(foreground_window_info.process_id)) {
        return {};
    }

    zaf::Handle pipe_handle{ 
        CreateFile(
            PipeName,
            GENERIC_READ | GENERIC_WRITE,
            0, 
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr)
    };

    if (!pipe_handle.IsValid()) {
        auto error = GetLastError();
        return {};
    }

    std::uint32_t response_content_length{};
    DWORD read_size{};
    BOOL is_succeeded = ReadFile(
        pipe_handle.Value(), 
        &response_content_length,
        sizeof(response_content_length),
        &read_size,
        nullptr);

    if (!is_succeeded) {
        return {};
    }

    auto buffer = std::make_unique<BYTE[]>(response_content_length);
    is_succeeded = ReadFile(
        pipe_handle.Value(), 
        buffer.get(), 
        response_content_length,
        &read_size, 
        nullptr);

    if (!is_succeeded) {
        auto last_error = GetLastError();
        return {};
    }

    std::string response((const char*)buffer.get(), read_size);
    return WindowBasedDiscoverer::DecodeActivePath(zaf::FromUtf8String(response));
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

}