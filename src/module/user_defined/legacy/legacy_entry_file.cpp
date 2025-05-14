#include <module/user_defined/legacy/legacy_entry_file.h>
#include "utility/app_directory.h"

namespace ra::mod::user_defined {

std::filesystem::path GetLegacyEntryFilePath() {
    return utility::GetUserDataDirectoryPath() / "UserDefined.txt";
}

}