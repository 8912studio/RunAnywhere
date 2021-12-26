#include "utility/clipboard.h"
#include <Windows.h>
#include <strsafe.h>

namespace ra::utility {
namespace {

DWORD CopyStringToMemory(const std::wstring& string, HGLOBAL memory_handle) {

    LPVOID memory = GlobalLock(memory_handle);
    if (memory == NULL) {
        return GetLastError();
    }

    HRESULT result = StringCchCopy(
        reinterpret_cast<wchar_t*>(memory),
        string.length() + 1,
        string.c_str());

    GlobalUnlock(memory_handle);

    if (FAILED(result)) {
        return ERROR_OUTOFMEMORY;
    }

    return ERROR_SUCCESS;
}


void InnerSetStringToClipboard(const std::wstring& string) {

    auto memory_size = (string.length() + 1) * sizeof(wchar_t);

    HGLOBAL memory_handle = GlobalAlloc(GMEM_MOVEABLE, memory_size);
    if (memory_handle == NULL) {
        return;
    }

    DWORD error = CopyStringToMemory(string, memory_handle);
    if (error != ERROR_SUCCESS) {
        GlobalFree(memory_handle);
        return;
    }

    SetClipboardData(CF_UNICODETEXT, memory_handle);
    GlobalFree(memory_handle);
}

}


void SetStringToClipboard(const std::wstring& string) {

    BOOL is_succeeded = OpenClipboard(NULL);
    if (!is_succeeded) {
        return;
    }

    EmptyClipboard();
    InnerSetStringToClipboard(string);
    CloseClipboard();
}

}