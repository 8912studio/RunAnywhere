#pragma once

#ifdef INSTALLERHELPER_EXPORTS
#define INSTALLERHELPER_API __declspec(dllexport)
#else
#define INSTALLERHELPER_API __declspec(dllimport)
#endif

extern "C" {

INSTALLERHELPER_API int __cdecl External_IsVisualStudioInstalled();

INSTALLERHELPER_API int __cdecl External_GetNPPInstalledDirectoryPath(
    wchar_t* buffer, 
    int buffer_size);

}