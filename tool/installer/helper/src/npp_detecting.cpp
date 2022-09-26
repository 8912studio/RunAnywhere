#include "npp_detecting.h"
#include <Windows.h>
#include <optional>
#include <vector>

namespace {

std::wstring ReadPathFromRegistry(bool read_from_32_key) {

    constexpr const wchar_t* NPPRegistryKeyPath = L"Software\\Notepad++";
    constexpr const wchar_t* NPPRegistryValueName = L"";
    const DWORD NPPRegistryFlags = 
        RRF_RT_REG_SZ | 
        (read_from_32_key ? RRF_SUBKEY_WOW6432KEY : RRF_SUBKEY_WOW6464KEY);

    DWORD buffer_size{ 128 };
    std::vector<std::byte> buffer(buffer_size);
    LSTATUS result = RegGetValue(
        HKEY_LOCAL_MACHINE,
        NPPRegistryKeyPath,
        NPPRegistryValueName,
        NPPRegistryFlags,
        nullptr,
        reinterpret_cast<BYTE*>(&buffer[0]),
        &buffer_size);

    if (result == ERROR_MORE_DATA) {

        buffer.resize(buffer_size);
        result = RegGetValue(
            HKEY_LOCAL_MACHINE,
            NPPRegistryKeyPath,
            NPPRegistryValueName,
            NPPRegistryFlags,
            nullptr,
            reinterpret_cast<BYTE*>(&buffer[0]),
            &buffer_size);
    }

    if (result != ERROR_SUCCESS) {
        return {};
    }

    buffer.resize(buffer_size);

    auto string = reinterpret_cast<const wchar_t*>(buffer.data());
    auto string_length = buffer.size() / sizeof(wchar_t);

    //The string buffer may or may not be terminated with \0.
    //Remove the last \0 if there is.
    if (string[string_length - 1] == L'\0') {
        --string_length;
    }

    std::wstring path{ string, string_length };

    //Remove the last \ delimiter.
    if (path.back() == L'\\') {
        path.pop_back();
    }

    return path;
}


NPPDetectResult Detect() {

    NPPDetectResult result;

    //Read x64 path first.
    result.path = ReadPathFromRegistry(false);
    if (result.path.empty()) {

        //Read x86 path if x64 path is not existent.
        result.path = ReadPathFromRegistry(true);
        result.is_x86 = true;
    }

    return result;
}

}


NPPDetectResult DetectNPPInstalledDirectoryPath() {

    static const NPPDetectResult result = Detect();
    return result;
}