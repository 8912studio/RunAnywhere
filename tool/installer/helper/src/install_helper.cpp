#include "install_helper.h"
#include "npp_detecting.h"
#include "uninstall.h"
#include "vs_detecting.h"
#include "vscode_detecting.h"

INSTALLERHELPER_API int __cdecl External_IsVS2019OrOlderInstalled() {
	return GetVSInstallationInfo().is_vs2019_or_older_installed;
}


INSTALLERHELPER_API int __cdecl External_IsVS2022OrNewerInstalled() {
	return GetVSInstallationInfo().is_vs2022_or_newer_installed;
}


INSTALLERHELPER_API void __cdecl External_GetVSIXInstallerPath(wchar_t* buffer, int buffer_size) {
	const auto& installation_info = GetVSInstallationInfo();
	wcsncpy_s(buffer, buffer_size, installation_info.vsix_installer_path.c_str(), buffer_size);
}


INSTALLERHELPER_API int __cdecl External_IsVSCodeInstalled() {
	return DetectIfVSCodeInstalled();
}


INSTALLERHELPER_API int __cdecl External_GetNPPInstalledDirectoryPath(
	wchar_t* buffer,
	int buffer_size) {

	auto result = DetectNPPInstalledDirectoryPath();
	wcsncpy_s(buffer, buffer_size, result.path.c_str(), buffer_size);
	return result.is_x86;
}


INSTALLERHELPER_API void __cdecl External_TerminateApplication(wchar_t* exe_path) {
	TerminateApplication(std::filesystem::path{ exe_path });
}