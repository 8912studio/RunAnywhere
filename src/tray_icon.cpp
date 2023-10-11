#include "tray_icon.h"
#include <shellapi.h>
#include "resource.h"

namespace ra {

#define ID_TRAY_ICON 1

NOTIFYICONDATA g_iconData = { 0 };


void AddTrayIcon(HWND hwnd, UINT message_id) {

	g_iconData.cbSize = sizeof(NOTIFYICONDATA);
	g_iconData.hWnd = hwnd;
	g_iconData.uID = ID_TRAY_ICON;
	g_iconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	g_iconData.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APPICON));
	g_iconData.uCallbackMessage = message_id;
	g_iconData.uVersion = NOTIFYICON_VERSION;
	wcscpy_s(g_iconData.szTip, L"RunAnywhere");

	Shell_NotifyIcon(NIM_ADD, &g_iconData);
}


void RemoveTrayIcon() {

	Shell_NotifyIcon(NIM_DELETE, &g_iconData);
}


void ShowBalloon(const std::wstring& title, const std::wstring& info) {

	auto icon_data = g_iconData;

	icon_data.uFlags = NIF_INFO;
	icon_data.dwInfoFlags = NIIF_INFO | NIIF_NOSOUND;
	icon_data.uTimeout = 3000;
	wcscpy_s(icon_data.szInfoTitle, title.c_str());
	wcscpy_s(icon_data.szInfo, info.c_str());

	Shell_NotifyIcon(NIM_MODIFY, &icon_data);
}


void KillTrayIconFocus() {

	Shell_NotifyIcon(NIM_SETFOCUS, &g_iconData);
}

}