#include "utility/app_directory.h"
#include <shlobj_core.h>
#include <zaf/base/error/com_error.h>

namespace ra::utility {
namespace {

std::filesystem::path GetAppPathInKnownDirectory(const GUID& guid) {

    wchar_t* buffer{};
    HRESULT result = SHGetKnownFolderPath(guid, 0, nullptr, &buffer);
    ZAF_THROW_IF_COM_ERROR(result);

    std::filesystem::path path{ buffer };
    CoTaskMemFree(buffer);

    return path / L"RunAnywhere";
}

}


std::filesystem::path GetUserDataDirectoryPath() {
    return GetAppPathInKnownDirectory(FOLDERID_RoamingAppData);
}


std::filesystem::path GetAppDataDirectoryPath() {
    return GetAppPathInKnownDirectory(FOLDERID_LocalAppData);
}

}