#pragma once

#ifdef INSTALLERHELPER_EXPORTS
#define INSTALLERHELPER_API __declspec(dllexport)
#else
#define INSTALLERHELPER_API __declspec(dllimport)
#endif

extern "C" {

INSTALLERHELPER_API int __cdecl External_IsVisualStudioInstalled();

INSTALLERHELPER_API void __cdecl External_GetVSCodeExePath(wchar_t* buffer, int buffer_size);

//Return value is 1 if Notepad++ is x86 version; 0 if Notepad++ is x64 version.
INSTALLERHELPER_API int __cdecl External_GetNPPInstalledDirectoryPath(
    wchar_t* buffer, 
    int buffer_size);

}