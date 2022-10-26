#include "utility/data_directory.h"
#include <shlobj_core.h>
#include <zaf/base/error/com_error.h>

namespace ra::utility {

std::filesystem::path GetDataDirectoryPath() {

    wchar_t* buffer{};
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &buffer);
    ZAF_THROW_IF_COM_ERROR(result);

    std::filesystem::path path{ buffer };
    CoTaskMemFree(buffer);

    return path / "RunAnywhere";
}

}