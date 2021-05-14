#pragma once

#include <string>
#include <vector>
#include "context/active_path.h"

namespace ra::context {

ActivePath DecodeActivePath(const std::wstring& encoded_path);

}