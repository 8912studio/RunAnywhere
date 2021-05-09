#pragma once

#ifdef INSTALLERHELPER_EXPORTS
#define INSTALLERHELPER_API __declspec(dllexport)
#else
#define INSTALLERHELPER_API __declspec(dllimport)
#endif

extern "C" {

INSTALLERHELPER_API int __cdecl IsVisualStudioInstalled();

}