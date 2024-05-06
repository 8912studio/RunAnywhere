#include "context/discover/window_based_discoverer.h"
#include <zaf/base/error/win32_error.h>
#include <zaf/base/string/split.h>
#include "common/window_based_discover.h"

namespace ra::context {
namespace {

constexpr const wchar_t* ClientWindowClassName = L"Zplutor.RunAnywhere.Discover.Client";

}


WindowBasedDiscoverer::~WindowBasedDiscoverer() {

    if (client_window_handle_) {
        DestroyWindow(client_window_handle_);
    }

    if (client_window_class_atom_) {
        UnregisterClass(reinterpret_cast<LPCWSTR>(client_window_class_atom_), nullptr);
    }
}


std::optional<ActivePath> WindowBasedDiscoverer::Discover(
    const ForegroundWindowInfo& foreground_window_info) {

    if (!TryToInitialize()) {
        return std::nullopt;
    }

    return DiscoverActivePath(foreground_window_info.process_id);
}


bool WindowBasedDiscoverer::TryToInitialize() {

    try {

        TryToRegisterClientWindowClass();
        TryToCreateClientWindow();
        return true;
    }
    catch (const zaf::Error&) {
        return false;
    }
}


void WindowBasedDiscoverer::TryToRegisterClientWindowClass() {

    if (client_window_class_atom_) {
        return;
    }

    WNDCLASSEX window_class{};
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.lpszClassName = ClientWindowClassName;
    window_class.lpfnWndProc = ClientWindowProcedure;
    client_window_class_atom_ = RegisterClassEx(&window_class);

    if (!client_window_class_atom_) {
        ZAF_THROW_WIN32_ERROR(GetLastError());
    }
}


void WindowBasedDiscoverer::TryToCreateClientWindow() {

    if (client_window_handle_) {
        return;
    }

    client_window_handle_ = CreateWindow(
        ClientWindowClassName,
        nullptr,
        0,
        0,
        0,
        0,
        0,
        HWND_MESSAGE,
        nullptr,
        nullptr,
        nullptr);

    if (!client_window_handle_) {
        ZAF_THROW_WIN32_ERROR(GetLastError());
    }

    SetLastError(0);
    SetWindowLongPtr(client_window_handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ZAF_THROW_IF_WIN32_ERROR(GetLastError());
}


LRESULT CALLBACK WindowBasedDiscoverer::ClientWindowProcedure(
    HWND hwnd,
    UINT message, 
    WPARAM wparam, 
    LPARAM lparam) {

    if (message == WM_COPYDATA) {

        auto discoverer = 
            reinterpret_cast<WindowBasedDiscoverer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (discoverer) {
            discoverer->ReceiveCopyDataMessage(*reinterpret_cast<COPYDATASTRUCT*>(lparam));
        }

        return TRUE;
    }

    return CallWindowProc(DefWindowProc, hwnd, message, wparam, lparam);
}


std::optional<ActivePath> WindowBasedDiscoverer::DiscoverActivePath(DWORD foreground_process_id) {

    response_buffer_.clear();
    bool has_found_foreground_window{};

    //Enumerate all host windows.
    HWND host_window_handle{};
    while (true) {

        host_window_handle = FindWindowEx(
            HWND_MESSAGE,
            host_window_handle,
            common::DiscoverHostWindowClassName,
            nullptr);

        if (!host_window_handle) {
            break;
        }

        //Skip host windows that are not foreground windows.
        DWORD host_process_id{};
        GetWindowThreadProcessId(host_window_handle, &host_process_id);
        if (host_process_id != foreground_process_id) {
            continue;
        }

        //Send a message to host window to discover the active path.
        current_sequence_ = ++sequence_seed_;

        //The result can be ignored. response_buffer_ is empty if it fails to send message, thus 
        //the active path is empty as well.
        SendMessageTimeout(
            host_window_handle,
            common::WM_DISCOVER,
            reinterpret_cast<WPARAM>(client_window_handle_),
            current_sequence_,
            SMTO_NORMAL,
            2000,
            nullptr);

        has_found_foreground_window = true;
        break;
    }

    if (has_found_foreground_window) {
        //Response data is ready when reaching here.
        return DecodeActivePath(response_buffer_);
    }

    return std::nullopt;
}


void WindowBasedDiscoverer::ReceiveCopyDataMessage(const COPYDATASTRUCT& copy_data_info) {

    if (copy_data_info.dwData != current_sequence_) {
        return;
    }

    auto data_pointer = reinterpret_cast<const wchar_t*>(copy_data_info.lpData);
    std::size_t data_length = copy_data_info.cbData / sizeof(wchar_t);

    response_buffer_.assign(data_pointer, data_length);
}


ActivePath WindowBasedDiscoverer::DecodeActivePath(const std::wstring& encoded) {

    auto paths = zaf::Split(encoded, L'|');
    if (paths.empty()) {
        return {};
    }

    const auto& path = paths[0];
    if (path.empty()) {
        return {};
    }

    std::filesystem::path workspace_path;
    if (paths.size() > 1) {
        workspace_path = paths[1];
    }

    return ActivePath{ path, workspace_path };
}

}