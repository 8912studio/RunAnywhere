#include "context/vs_discovering.h"
#include <Windows.h>
#include <zaf/base/error/system_error.h>
#include "context/active_path_decoding.h"

namespace ra::context {
namespace {

constexpr const wchar_t* const HostWindowClassName = L"Zplutor.RunAnywhere.VS.Host";
constexpr const wchar_t* const ClientWindowClassName = L"Zplutor.Runanywhere.VS.Client";
constexpr UINT WM_REQUEST_PATH = WM_USER + 1;

ATOM g_client_window_class_atom{};
HWND g_client_window_handle{};
std::wstring g_temp_paths{};


LRESULT CALLBACK ClientWindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

    if (message == WM_COPYDATA) {

        auto copy_data_struct = reinterpret_cast<COPYDATASTRUCT*>(lparam);
        if (copy_data_struct->dwData == reinterpret_cast<ULONG_PTR>(g_client_window_handle)) {

            g_temp_paths.assign(
                reinterpret_cast<const wchar_t*>(copy_data_struct->lpData),
                copy_data_struct->cbData / 2);

            return 0;
        }
    }

    return CallWindowProc(DefWindowProc, hwnd, message, wparam, lparam);
}


void TryToRegisterClientWindowClass() {

    if (g_client_window_class_atom) {
        return;
    }

    WNDCLASSEX window_class{};
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.lpszClassName = ClientWindowClassName;
    window_class.lpfnWndProc = ClientWindowProcedure;
    g_client_window_class_atom = RegisterClassEx(&window_class);

    if (!g_client_window_class_atom) {
        ZAF_THROW_SYSTEM_ERROR(GetLastError());
    }
}


void TryToCreateClientWindow() {

    if (g_client_window_handle) {
        return;
    }

    g_client_window_handle = CreateWindow(
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

    if (!g_client_window_handle) {
        ZAF_THROW_SYSTEM_ERROR(GetLastError());
    }
}


std::wstring GetEncodedPathsFromHosts(HWND foreground_window_handle) {

    g_temp_paths.clear();

    DWORD foreground_process_id{};
    GetWindowThreadProcessId(foreground_window_handle, &foreground_process_id);

    //Enumerate all host windows.
    HWND host_window_handle{};
    while (true) {

        host_window_handle = FindWindowEx(
            HWND_MESSAGE,
            host_window_handle,
            HostWindowClassName,
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

        //Send a message to host window to request the focused path.
        DWORD result{};
        SendMessageTimeout(
            host_window_handle,
            WM_REQUEST_PATH,
            reinterpret_cast<WPARAM>(g_client_window_handle),
            0,
            SMTO_NORMAL,
            2000,
            &result);

        break;
    }

    return g_temp_paths;
}

}


ActivePath DiscoverActivePathFromVS(HWND foreground_window_handle) {

    try {

        TryToRegisterClientWindowClass();
        TryToCreateClientWindow();

        auto encoded_path = GetEncodedPathsFromHosts(foreground_window_handle);
        return DecodeActivePath(encoded_path);
    }
    catch (const zaf::Error&) {
        return {};
    }
}

}