#pragma once

#ifdef INSTALLERHELPER_EXPORTS
#define INSTALLERHELPER_API __declspec(dllexport)
#else
#define INSTALLERHELPER_API __declspec(dllimport)
#endif

extern "C" {

INSTALLERHELPER_API int __cdecl External_IsVS2019OrOlderInstalled();
INSTALLERHELPER_API int __cdecl External_IsVS2022OrNewerInstalled();
INSTALLERHELPER_API void __cdecl External_GetVSIXInstallerPath(wchar_t* buffer, int buffer_size);

INSTALLERHELPER_API int __cdecl External_IsVSCodeInstalled();

//Return value is 1 if Notepad++ is x86 version; 0 if Notepad++ is x64 version.
INSTALLERHELPER_API int __cdecl External_GetNPPInstalledDirectoryPath(
    wchar_t* buffer, 
    int buffer_size);

INSTALLERHELPER_API void __cdecl External_TerminateApplication(wchar_t* exe_path);

}