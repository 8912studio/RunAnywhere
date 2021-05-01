#pragma once

#include <Windows.h>

namespace ra {

void AddTrayIcon(HWND hwnd, UINT message_id);
void RemoveTrayIcon();
void ShowBalloonTips(LPCTSTR content, DWORD iconFlag = NIIF_INFO);
void KillTrayIconFocus();

}