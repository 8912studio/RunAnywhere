#include <module/user_defined/legacy/legacy_entry_file.h>
#include "utility/data_directory.h"

namespace ra::module::user_defined {

std::filesystem::path GetLegacyEntryFilePath() {
    return utility::GetDataDirectoryPath() / "UserDefined.txt";
}

}