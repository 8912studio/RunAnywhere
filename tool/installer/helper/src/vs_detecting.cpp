#include "vs_detecting.h"
#include <Windows.h>
#include <atlbase.h>
#include <comutil.h>
#include <cwchar>
#include <Setup.Configuration.h>

namespace {

bool IsSupportedVersion(const wchar_t* version) {

    wchar_t* end;
    auto major = std::wcstol(version, &end, 10);

    if (*end != L'.') {
        return false;
    }

    return major >= 15;
}


bool InnerDetect() {

    CComPtr<ISetupConfiguration> configuration;
    HRESULT hresult = CoCreateInstance(
        __uuidof(SetupConfiguration),
        nullptr, 
        CLSCTX_INPROC_SERVER, 
        __uuidof(ISetupConfiguration),
        reinterpret_cast<LPVOID*>(&configuration));

    if (FAILED(hresult)) {
        return false;
    }

    CComPtr<IEnumSetupInstances> instance_enumerator;
    hresult = configuration->EnumInstances(&instance_enumerator);
    if (FAILED(hresult)) {
        return false;
    }

    CComPtr<ISetupInstance> instance;
    while (true) {

        hresult = instance_enumerator->Next(1, &instance, nullptr);
        if (hresult != S_OK) {
            break;
        }

        bstr_t version{};
        hresult = instance->GetInstallationVersion(version.GetAddress());
        if (SUCCEEDED(hresult)) {

            if (IsSupportedVersion(version)) {
                return true;
            }
        }

        instance.Release();
    }

    return false;
}


bool Detect() {

    CoInitialize(nullptr);
    bool is_installed = InnerDetect();
    CoUninitialize();
    return is_installed;
}

}


bool DetectIfVisualStudioInstalled() {

    static const bool is_installed = Detect();
	return is_installed;
}