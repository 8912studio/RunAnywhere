#include "install_helper.h"
#include "npp_detecting.h"
#include "vs_detecting.h"

int __cdecl External_IsVisualStudioInstalled() {
	return DetectIfVisualStudioInstalled();
}


int __cdecl External_GetNPPInstalledDirectoryPath(wchar_t* buffer, int buffer_size) {

	auto result = DetectNPPInstalledDirectoryPath();
	wcsncpy_s(buffer, buffer_size, result.path.data(), buffer_size);
	return result.is_x86;
}