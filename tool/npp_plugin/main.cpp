#include <Windows.h>
#include <array>
#include <cwchar>
#include "npp_plugin.h"
#include "PluginInterface.h"

constexpr UINT WM_REQUEST = WM_USER + 1;

NppData g_npp_data{};
FuncItem g_menu_items[1]{};

HWND g_window_handle{};


extern "C" __declspec(dllexport) void setInfo(NppData npp_data) {
    g_npp_data = npp_data;
}


extern "C" __declspec(dllexport) const TCHAR* getName() {
    return L"RunAnywhere";
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
        L"RunAnywhere", 
        L"A plugin to discover active path for RunAnywhere", 
        MB_OK);
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

    if (message == WM_REQUEST) {

        wchar_t path_buffer[MAX_PATH]{};
        SendMessage(
            g_npp_data._nppHandle, 
            NPPM_GETFULLCURRENTPATH, 
            MAX_PATH,
            reinterpret_cast<LPARAM>(path_buffer));

        COPYDATASTRUCT copy_data_struct{};
        copy_data_struct.dwData = wparam;
        copy_data_struct.cbData = static_cast<DWORD>(std::wcslen(path_buffer) * sizeof(wchar_t));
        copy_data_struct.lpData = path_buffer;
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
    window_class.lpszClassName = NPPPluginHostWindowClassName;
    window_class.lpfnWndProc = WindowProcedure;

    ATOM atom = RegisterClassEx(&window_class);
    if (!atom) {
        return;
    }

    g_window_handle = CreateWindow(
        NPPPluginHostWindowClassName,
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