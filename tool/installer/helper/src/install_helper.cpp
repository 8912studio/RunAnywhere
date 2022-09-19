#include "install_helper.h"
#include "npp_detecting.h"
#include "vs_detecting.h"

int __cdecl External_IsVisualStudioInstalled() {
	return DetectIfVisualStudioInstalled();
}


int __cdecl External_GetNPPInstalledDirectoryPath(wchar_t* buffer, int buffer_size) {

	auto path = DetectNPPInstalledDirectoryPath();
	wcsncpy_s(buffer, buffer_size, path.data(), buffer_size);
	return wcslen(buffer);
}