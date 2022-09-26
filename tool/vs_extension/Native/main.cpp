#include <Windows.h>
#include <fstream>
#include <functional>
#include <string>
#include <thread>
#include "common/window_based_discover.h"

namespace {

using RequestEvent = void(*)();

HMODULE g_module_handle{};
HANDLE g_pipe_handle{};
RequestEvent g_request_event{};
std::unique_ptr<std::thread> g_thread;
HWND g_window_handle{};

std::ofstream g_log;


void InitializeLog() {

    wchar_t buffer[MAX_PATH]{};
    GetModuleFileName(g_module_handle, buffer, MAX_PATH);

    std::wstring directory_path;
    auto delimiter = wcsrchr(buffer, L'\\');
    if (delimiter == nullptr) {
        directory_path = buffer;
    }
    else {
        directory_path.assign(buffer, delimiter - buffer);
    }

    auto log_file_path = directory_path + L"\\Log.txt";
    g_log.open(log_file_path, std::ios::out | std::ios::trunc);
}


std::wstring ReadPathFromPipe() {

    DWORD path_length{};
    DWORD read_length{};
    BOOL is_succeeded = ReadFile(
        g_pipe_handle, 
        &path_length, 
        sizeof(path_length), 
        &read_length, 
        nullptr);

    if (!is_succeeded) {
        //g_log << "Read path length failed. " << GetLastError() << std::endl;
        return {};
    }

    //g_log << "Read path length: " << path_length << std::endl;
    if (path_length == 0) {
        return {};
    }

    auto buffer = std::make_unique<wchar_t[]>(path_length);
    is_succeeded = ReadFile(
        g_pipe_handle, 
        buffer.get(),
        path_length * sizeof(wchar_t),
        &read_length, 
        nullptr);

    if (!is_succeeded) {
        //g_log << "Read path failed. " << GetLastError() << std::endl;
        return {};
    }

    std::wstring path(buffer.get(), read_length / 2);
    //g_log << "Read path succeeded. " << path.length() << std::endl;

    return path;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

    if (message == ra::common::WM_DISCOVER) {

        //g_log << "Notify request event." << std::endl;
        g_request_event();

        //g_log << "Read path from pipe." << std::endl;
        auto path = ReadPathFromPipe();

        COPYDATASTRUCT copy_data_struct{};
        copy_data_struct.dwData = lparam; //Sequence
        copy_data_struct.cbData = path.length() * sizeof(wchar_t);
        copy_data_struct.lpData = path.data();
        SendMessage(
            reinterpret_cast<HWND>(wparam),
            WM_COPYDATA, 
            0, 
            reinterpret_cast<LPARAM>(&copy_data_struct));

        return 0;
    }

    return CallWindowProc(DefWindowProc, hwnd, message, wparam, lparam);
}


void ThreadProcedure() {

    //Register class
    WNDCLASSEX window_class{};
    window_class.cbSize = sizeof(window_class);
    window_class.lpszClassName = ra::common::DiscoverHostWindowClassName;
    window_class.lpfnWndProc = WindowProcedure;

    ATOM atom = RegisterClassEx(&window_class);
    if (!atom) {
        return;
    }

    g_window_handle = CreateWindow(
        ra::common::DiscoverHostWindowClassName,
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

    if (!g_window_handle) {
        return;
    }

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        DispatchMessage(&msg);
    }

    DestroyWindow(g_window_handle);
}

}


extern "C" __declspec(dllexport) void __stdcall RunAnywhereVSHost_Initialize(
    HANDLE pipe_handle, 
    RequestEvent request_event) {

    //InitializeLog();
    //g_log << "RunAnywhereVSHost_Initialize " << pipe_handle << ' ' << request_event << std::endl;

    if (!pipe_handle || !request_event) {
        return;
    }

    g_pipe_handle = pipe_handle;
    g_request_event = request_event;
    g_thread = std::make_unique<std::thread>(ThreadProcedure);
}


extern "C" __declspec(dllexport) void __stdcall RunAnywhereVSHost_Uninitialize() {

    if (g_window_handle) {
        PostMessage(g_window_handle, WM_QUIT, 0, 0);
    }

    if (g_thread->joinable()) {
        g_thread->join();
    }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {

    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_module_handle = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}