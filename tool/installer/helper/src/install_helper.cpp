#include "install_helper.h"
#include "npp_detecting.h"
#include "vs_detecting.h"
#include "vscode_detecting.h"

INSTALLERHELPER_API int __cdecl External_IsVS2019OrOlderInstalled() {
	return GetVSInstallationInfo().version_type == VSVersionType::VS2019OrOlder;
}


INSTALLERHELPER_API int __cdecl External_IsVS2022OrNewerInstalled() {
	return GetVSInstallationInfo().version_type == VSVersionType::VS2022OrNewer;
}


INSTALLERHELPER_API void __cdecl External_GetVSIXInstallerPath(wchar_t* buffer, int buffer_size) {
	const auto& installation_info = GetVSInstallationInfo();
	wcsncpy_s(buffer, buffer_size, installation_info.vsix_installer_path.c_str(), buffer_size);
}


int __cdecl External_IsVSCodeInstalled() {
	return DetectIfVSCodeInstalled();
}


int __cdecl External_GetNPPInstalledDirectoryPath(wchar_t* buffer, int buffer_size) {

	auto result = DetectNPPInstalledDirectoryPath();
	wcsncpy_s(buffer, buffer_size, result.path.c_str(), buffer_size);
	return result.is_x86;
}