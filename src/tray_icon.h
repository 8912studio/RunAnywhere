#pragma once

#include <Windows.h>
#include <string>

namespace ra {

void AddTrayIcon(HWND hwnd, UINT message_id);
void RemoveTrayIcon();
void ShowBalloon(const std::wstring& title, const std::wstring& info);
void KillTrayIconFocus();

}