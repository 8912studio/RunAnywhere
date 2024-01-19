#pragma once

#include <Windows.h>
#include "module/extension/sdk/run_anywhere_extension_sdk.h"

namespace ra::mod::extension {

class ExtensionInterface {
public:
    HMODULE module_handle{};
    decltype(&RA_Create) create_function{};
    decltype(&RA_Interpret) interpret_function{};
    decltype(&RA_GetPreviewText) get_preview_text_function{};
    decltype(&RA_Destroy) destroy_function{};
};

}