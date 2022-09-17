#include <Windows.h>
#include <array>
#include <cwchar>
#include <string>
#include "common/window_based_discover.h"
#include "PluginInterface.h"

NppData g_npp_data{};
FuncItem g_menu_items[1]{};

HWND g_window_handle{};


extern "C" __declspec(dllexport) void setInfo(NppData npp_data) {
    g_npp_data = npp_data;
}


extern "C" __declspec(dllexport) const TCHAR* getName() {
    return L"RunAnywhereNPPHost";
}


extern "C" __declspec(dllexport) FuncItem* getFuncsArray(int* count) {
    *count = static_cast<int>(std::size(g_menu_items));
    return g_menu_items;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification*) {

}


extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam) {
    return TRUE;
}


extern "C" __declspec(dllexport) BOOL isUnicode() {
    return TRUE;
}


void About() {

    ::MessageBox(
        g_npp_data._nppHandle, 
        L"A plugin to discover active path for RunAnywhere",
        L"RunAnywhereNPPHost", 
        MB_OK);
}


std::wstring GetActivePath() {

    wchar_t path_buffer[MAX_PATH]{};

    //Get directory of current document first to determinate whether it is a real file on disk.
    //Because NPPM_GETFULLCURRENTPATH will return an non-empty string even if current document is
    //not a real file.
    SendMessage(
        g_npp_data._nppHandle,
        NPPM_GETCURRENTDIRECTORY,
        MAX_PATH,
        reinterpret_cast<LPARAM>(path_buffer));

    if (std::wcslen(path_buffer) == 0) {
        return {};
    }

    SendMessage(
        g_npp_data._nppHandle,
        NPPM_GETFULLCURRENTPATH,
        MAX_PATH,
        reinterpret_cast<LPARAM>(path_buffer));

    return std::wstring{ path_buffer };
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

    if (message == ra::common::WM_DISCOVER) {

        auto active_path = GetActivePath();

        COPYDATASTRUCT copy_data_struct{};
        copy_data_struct.dwData = lparam; //Sequence
        copy_data_struct.cbData = static_cast<DWORD>(active_path.length() * sizeof(wchar_t));
        copy_data_struct.lpData = active_path.data();
        SendMessage(
            reinterpret_cast<HWND>(wparam),
            WM_COPYDATA,
            0,
            reinterpret_cast<LPARAM>(&copy_data_struct));

        return 0;
    }

    return CallWindowProc(DefWindowProc, hwnd, message, wparam, lparam);
}


void Initialize() {

    auto& menu_item = g_menu_items[0];
    wcscpy_s(menu_item._itemName, L"About");
    menu_item._pFunc = About;

    //Register window class
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
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        Initialize();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}