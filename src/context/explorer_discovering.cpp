#include "context/explorer_discovering.h"
#include <atlbase.h>
#include <exdisp.h>
#include <shlobj.h>
#include <zaf/base/error/com_error.h>

namespace ra::context {
namespace {

//This approach is not 100% accurate, but it is suitable in most cases.
bool IsDesktopForeground(HWND foreground_window_handle) {

    auto shell_window_handle = GetShellWindow();
    if (!shell_window_handle) {
        return false;
    }

    DWORD shell_window_process_id{};
    GetWindowThreadProcessId(shell_window_handle, &shell_window_process_id);

    DWORD foreground_window_process_id{};
    GetWindowThreadProcessId(foreground_window_handle, &foreground_window_process_id);

    return shell_window_process_id == foreground_window_process_id;
}


CComPtr<IWebBrowser2> FindForegroundDesktopWindow(
    IShellWindows* shell_windows,
    HWND foreground_window_handle) {

    if (!IsDesktopForeground(foreground_window_handle)) {
        return nullptr;
    }

    VARIANT pvarLoc{};
    VARIANT pvarLocRoot{};
    long desktop_handle{};
    CComPtr<IDispatch> dispatch;
    auto hresult = shell_windows->FindWindowSW(
        &pvarLoc, 
        &pvarLocRoot, 
        SWC_DESKTOP, 
        &desktop_handle,
        SWFO_NEEDDISPATCH, 
        &dispatch);

    if (FAILED(hresult)) {
        return nullptr;
    }

    CComPtr<IWebBrowser2> browser;
    hresult = dispatch->QueryInterface(&browser);
    if (FAILED(hresult)) {
        return nullptr;
    }

    return browser;
}


CComPtr<IWebBrowser2> FindForegroundExplorerWindow(
    IShellWindows* shell_windows,
    HWND foreground_window_handle) {

    for (int index = 0; ; ++index) {

        CComVariant variant_index = index;

        IDispatch* dispatch{};
        HRESULT hresult = shell_windows->Item(variant_index, &dispatch);
        if (FAILED(hresult) || hresult == S_FALSE) {
            break;
        }

        CComPtr<IWebBrowser2> browser;
        hresult = dispatch->QueryInterface(&browser);
        if (FAILED(hresult)) {
            continue;
        }

        SHANDLE_PTR handle{};
        hresult = browser->get_HWND(&handle);
        if (FAILED(hresult)) {
            continue;
        }

        if (reinterpret_cast<HWND>(handle) == foreground_window_handle) {
            return browser;
        }
    }

    return nullptr;
}


CComPtr<IWebBrowser2> FindForegroundWindow(HWND foreground_window_handle) {

    CComPtr<IShellWindows> shell_windows;
    HRESULT hresult = CoCreateInstance(
        CLSID_ShellWindows,
        NULL,
        CLSCTX_ALL,
        IID_IShellWindows,
        (void**)&shell_windows);

    ZAF_THROW_IF_COM_ERROR(hresult);

    auto result = FindForegroundExplorerWindow(shell_windows, foreground_window_handle);
    if (result) {
        return result;
    }

    return FindForegroundDesktopWindow(shell_windows, foreground_window_handle);
}


std::wstring GetFolderPath(IPersistFolder2* persist_folder) {

    LPITEMIDLIST folder_pidl{};
    HRESULT hresult = persist_folder->GetCurFolder(&folder_pidl);
    ZAF_THROW_IF_COM_ERROR(hresult);

    wchar_t buffer[MAX_PATH]{};
    BOOL is_succeeded = SHGetPathFromIDList(folder_pidl, buffer);

    CoTaskMemFree(folder_pidl);

    if (is_succeeded) {
        return buffer;
    }
    else {
        return {};
    }
}


std::wstring GetSelectedItemName(IFolderView* folder_view, IPersistFolder2* persist_folder) {

    CComPtr<IFolderView2> folder_view2;
    HRESULT hresult = folder_view->QueryInterface(&folder_view2);
    ZAF_THROW_IF_COM_ERROR(hresult);

    int selected_item_index{};
    hresult = folder_view2->GetSelectedItem(0, &selected_item_index);
    ZAF_THROW_IF_COM_ERROR(hresult);
    if (hresult == S_FALSE) {
        return {};
    }

    CComPtr<IShellFolder> shell_folder;
    hresult = persist_folder->QueryInterface(&shell_folder);
    ZAF_THROW_IF_COM_ERROR(hresult);

    LPITEMIDLIST item_pidl{};
    hresult = folder_view->Item(selected_item_index, &item_pidl);
    ZAF_THROW_IF_COM_ERROR(hresult);

    STRRET strret{};
    hresult = shell_folder->GetDisplayNameOf(item_pidl, SHGDN_INFOLDER | SHGDN_FORPARSING, &strret);

    std::wstring result;
    if (SUCCEEDED(hresult)) {

        wchar_t buffer[MAX_PATH]{};
        StrRetToBuf(&strret, item_pidl, buffer, MAX_PATH);
        result = buffer;
    }

    CoTaskMemFree(item_pidl);
    return result;
}

}


std::filesystem::path DiscoverActivePathFromExplorer(HWND foreground_window_handle) {

    //Reference: https://devblogs.microsoft.com/oldnewthing/?p=38393

    try {

        auto foreground_window = FindForegroundWindow(foreground_window_handle);
        if (!foreground_window) {
            return {};
        }

        CComPtr<IServiceProvider> service_provider;
        HRESULT hresult = foreground_window->QueryInterface(&service_provider);
        ZAF_THROW_IF_COM_ERROR(hresult);

        CComPtr<IShellBrowser> shell_browser;
        hresult = service_provider->QueryService(SID_STopLevelBrowser, &shell_browser);
        ZAF_THROW_IF_COM_ERROR(hresult);

        CComPtr<IShellView> shell_view;
        hresult = shell_browser->QueryActiveShellView(&shell_view);
        ZAF_THROW_IF_COM_ERROR(hresult);

        CComPtr<IFolderView> folder_view;
        hresult = shell_view->QueryInterface(&folder_view);
        ZAF_THROW_IF_COM_ERROR(hresult);

        CComPtr<IPersistFolder2> persist_folder;
        hresult = folder_view->GetFolder(
            IID_IPersistFolder2,
            reinterpret_cast<void**>(&persist_folder));
        ZAF_THROW_IF_COM_ERROR(hresult);

        auto folder_path = GetFolderPath(persist_folder);
        if (folder_path.empty()) {
            return {};
        }

        auto selected_item_name = GetSelectedItemName(folder_view, persist_folder);
        return std::filesystem::path(folder_path) / selected_item_name;
    }
    catch (const zaf::Error&) {
        return {};
    }
}

}