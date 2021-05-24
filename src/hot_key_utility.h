#pragma once

#include <string>
#include "hot_key.h"

namespace ra {

bool IsFunctionalKey(UINT virtual_key);
bool IsIndependentKey(UINT virtual_key);
bool IsDependentKey(UINT virtual_key);

std::wstring GenerateTextByHotKey(const HotKey& hot_key);

}