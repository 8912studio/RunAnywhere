#pragma once

#include <Windows.h>
#include "module/extension/sdk/run_anywhere_extension_sdk.h"

namespace ra::module::extension {

class ExtensionInterface {
public:
    HMODULE module_handle{};
    decltype(&RunAnywhere_Interpret) interpret_function{};
    decltype(&RunAnywhere_GetText) get_text_function{};
    decltype(&RunAnywhere_Destroy) destroy_function{};
};

}