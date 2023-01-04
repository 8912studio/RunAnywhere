#include "context/discover/vscode_discoverer.h"
#include <zaf/base/handle.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/encoding_conversion.h>
#include "context/discover/window_based_discoverer.h"

namespace ra::context {
namespace {

constexpr const wchar_t* PipeName = LR"(\\.\pipe\Zplutor.RunAnywhere.Discover.VSCode)";

}


VSCodeDiscoverer::VSCodeDiscoverer() {

}


std::optional<ActivePath> VSCodeDiscoverer::Discover(
    const ForegroundWindowInfo& foreground_window_info) {

    if (!IsVSCodeProcess(foreground_window_info.process_id)) {
        return std::nullopt;
    }

    return GetActivePathFromVSCode();
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


ActivePath VSCodeDiscoverer::GetActivePathFromVSCode() {

    zaf::Handle pipe_handle{
        CreateFile(
            PipeName,
            GENERIC_READ,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr)
    };

    if (!pipe_handle.IsValid()) {
        return {};
    }

    std::uint32_t response_content_size{};
    DWORD read_size{};
    BOOL is_succeeded = ReadFile(
        pipe_handle.Value(),
        &response_content_size,
        sizeof(response_content_size),
        &read_size,
        nullptr);

    if (!is_succeeded) {
        return {};
    }

    if (response_content_size == 0) {
        return {};
    }

    auto buffer = std::make_unique<std::byte[]>(response_content_size);
    is_succeeded = ReadFile(
        pipe_handle.Value(),
        buffer.get(),
        response_content_size,
        &read_size,
        nullptr);

    if (!is_succeeded) {
        return {};
    }

    std::string response_content(reinterpret_cast<const char*>(buffer.get()), read_size);
    return WindowBasedDiscoverer::DecodeActivePath(zaf::FromUTF8String(response_content));
}

}