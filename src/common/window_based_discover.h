#pragma once

#include <Windows.h>

namespace ra::common {

constexpr const wchar_t* DiscoverHostWindowClassName = L"Zplutor.RunAnywhere.Discover.Host";

constexpr UINT WM_DISCOVER = WM_USER + 1;

}