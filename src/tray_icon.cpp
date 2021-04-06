#include "tray_icon.h"
#include "../resource.h"

#define ID_TRAY_ICON 1

NOTIFYICONDATA g_iconData = { 0 };



void AddTrayIcon(HWND hwnd, UINT message_id) {

	g_iconData.cbSize = sizeof(NOTIFYICONDATA);
	g_iconData.hWnd = hwnd;
	g_iconData.uID = ID_TRAY_ICON;
	g_iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_iconData.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
	g_iconData.uCallbackMessage = message_id;
	g_iconData.uVersion = NOTIFYICON_VERSION;
	wcscpy_s(g_iconData.szInfoTitle, L"RunAnywhere");

	Shell_NotifyIcon(NIM_ADD, &g_iconData);
}



void RemoveTrayIcon() {

	Shell_NotifyIcon(NIM_DELETE, &g_iconData);
}



void ShowBalloonTips(LPCTSTR content, DWORD iconFlag) {

	g_iconData.uFlags |= NIF_INFO;
	g_iconData.uTimeout = 3000;
	g_iconData.dwInfoFlags = iconFlag;
	wcscpy_s(g_iconData.szInfo, content);

	Shell_NotifyIcon(NIM_MODIFY, &g_iconData);
}



void KillTrayIconFocus() {

	Shell_NotifyIcon(NIM_SETFOCUS, &g_iconData);
}