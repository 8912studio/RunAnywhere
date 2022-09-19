#include "npp_detecting.h"
#include <Windows.h>
#include <vector>

std::wstring DetectNPPInstalledDirectoryPath() {

    //Note: The code here to read registry should be replaced with zaf::Registry component once Zaf
    //supports x86 and MT runtime.

    constexpr const wchar_t* NPPRegistryKeyPath = L"Software\\Notepad++";

    DWORD value_type{};
    DWORD buffer_size{ 128 };
    std::vector<std::byte> buffer(buffer_size);
    LSTATUS result = RegGetValue(
        HKEY_LOCAL_MACHINE,
        NPPRegistryKeyPath,
        L"",
        RRF_RT_REG_SZ,
        &value_type,
        reinterpret_cast<BYTE*>(&buffer[0]),
        &buffer_size);

    if (result == ERROR_MORE_DATA) {

        buffer.resize(buffer_size);
        result = RegGetValue(
            HKEY_LOCAL_MACHINE,
            NPPRegistryKeyPath,
            L"",
            RRF_RT_REG_SZ,
            &value_type,
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

    std::wstring directory_path{ string, string_length };

    //Remove the last \ delimiter.
    if (directory_path.back() == L'\\') {
        directory_path.pop_back();
    }

    return directory_path;
}