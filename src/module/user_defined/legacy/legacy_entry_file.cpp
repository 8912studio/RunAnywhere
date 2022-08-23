#include <module/user_defined/legacy/legacy_entry_file.h>
#include <Windows.h>
#include <shlobj_core.h>

namespace ra::module::user_defined {

std::filesystem::path GetLegacyEntryFilePath() {

    wchar_t* buffer{};
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &buffer);
    if (FAILED(result)) {
        return {};
    }

    std::filesystem::path path{ buffer };
    CoTaskMemFree(buffer);

    path = path / "RunAnywhere" / "UserDefined.txt";
    return path;
}

}