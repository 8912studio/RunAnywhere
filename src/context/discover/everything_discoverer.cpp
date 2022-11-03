#include "context/discover/everything_discoverer.h"
#include <CommCtrl.h>
#include <everything_ipc.h>
#include <zaf/base/handle.h>

namespace ra::context {
namespace {

bool IsEverythingWindowHandle(HWND window_handle) {

    //Use ANSI version to get char count.
    constexpr int class_name_length = sizeof(EVERYTHING_IPC_SEARCH_CLIENT_WNDCLASSA);

    wchar_t class_name[class_name_length]{};
    GetClassName(window_handle, class_name, class_name_length);

    return std::wcscmp(class_name, EVERYTHING_IPC_SEARCH_CLIENT_WNDCLASS) == 0;
}


std::wstring ReadTextFromListViewItem(
    HWND list_view_handle,
    HANDLE process_handle,
    LPVOID process_buffer,
    int item_index,
    int sub_item_index) {

    LVITEM item_info{};
    item_info.mask = LVIF_TEXT;
    item_info.iItem = item_index;
    item_info.iSubItem = sub_item_index;
    item_info.pszText = reinterpret_cast<wchar_t*>(
        reinterpret_cast<std::uint8_t*>(process_buffer) + sizeof(LVITEM));
    item_info.cchTextMax = MAX_PATH;

    SIZE_T written_size{};
    BOOL is_succeeded = WriteProcessMemory(
        process_handle,
        process_buffer,
        &item_info,
        sizeof(item_info),
        &written_size);

    if (!is_succeeded) {
        return {};
    }

    is_succeeded = ListView_GetItem(list_view_handle, reinterpret_cast<LPARAM>(process_buffer));
    if (!is_succeeded) {
        return {};
    }

    SIZE_T read_size{};
    is_succeeded = ReadProcessMemory(
        process_handle,
        process_buffer,
        &item_info,
        sizeof(item_info),
        &read_size);

    if (!is_succeeded) {
        return {};
    }

    auto text_buffer = std::make_unique<wchar_t[]>(item_info.cchTextMax);
    is_succeeded = ReadProcessMemory(
        process_handle,
        item_info.pszText,
        text_buffer.get(),
        item_info.cchTextMax,
        &read_size);

    if (!is_succeeded) {
        return {};
    }

    return text_buffer.get();
}


ActivePath GetActivePathFromEverything(const ForegroundWindowInfo& foreground_window_info) {

    HWND list_view_handle = FindWindowEx(
        foreground_window_info.window_handle,
        nullptr,
        WC_LISTVIEW,
        nullptr);

    if (!list_view_handle) {
        return {};
    }

    auto selected_index = ListView_GetNextItem(list_view_handle, -1, LVNI_SELECTED);
    if (selected_index < 0) {
        return {};
    }

    DWORD process_id{};
    GetWindowThreadProcessId(list_view_handle, &process_id);
    if (!process_id) {
        return {};
    }

    DWORD desired_access =
        PROCESS_QUERY_LIMITED_INFORMATION |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE |
        PROCESS_VM_READ;

    zaf::Handle process_handle{ OpenProcess(desired_access, FALSE, process_id) };
    if (!process_handle) {
        return {};
    }

    BOOL is_wow64_process{};
    if (!IsWow64Process(*process_handle, &is_wow64_process)) {
        return {};
    }

    //32bit Everything process is not supported.
    if (is_wow64_process) {
        return {};
    }

    std::size_t process_buffer_size = sizeof(LVITEM) + sizeof(wchar_t) * MAX_PATH;
    auto process_buffer = VirtualAllocEx(
        *process_handle,
        nullptr,
        process_buffer_size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);

    if (!process_buffer) {
        return {};
    }

    std::filesystem::path file_name = ReadTextFromListViewItem(
        list_view_handle,
        *process_handle,
        process_buffer,
        selected_index,
        0);

    //Assume that the "Path" column is visible.
    //If not, it is unable to get path from Everything.
    std::filesystem::path directory_path = ReadTextFromListViewItem(
        list_view_handle,
        *process_handle,
        process_buffer,
        selected_index,
        1);

    VirtualFreeEx(*process_handle, process_buffer, 0, MEM_RELEASE);

    if (file_name.empty() || directory_path.empty()) {
        return {};
    }

    auto full_path = directory_path / file_name;

    std::error_code error_code;
    if (!std::filesystem::exists(full_path, error_code)) {
        return {};
    }

    return ActivePath{ full_path };
}

}


std::optional<ActivePath> EverythingDiscoverer::Discover(
    const ForegroundWindowInfo& foreground_window_info) {

    if (!IsEverythingWindowHandle(foreground_window_info.window_handle)) {
        return std::nullopt;
    }

    return GetActivePathFromEverything(foreground_window_info);
}

}