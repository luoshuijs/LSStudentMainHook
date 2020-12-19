#include "pch.h"
#include "framework.h"
#include "LSWindows.h"
#include "IATHOOK.h"
#include "MyHook.h"

IATHOOK CreateWindowExW_HOOK;
CREATEWINDOWEXW OldCreateWindowExW = NULL;

HWND MyCreateWindowExW(
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
) {
	if (lpWindowName != NULL) {
		if (CompareStringOrdinal(lpClassName, -1, L"TDDesk Render Window", -1, TRUE) == CSTR_EQUAL) {
			//WindowName = "TDDesk Render Window"
			//TopDomain Test Quiz Grade Notify Window
			//Style = WS_OVERLAPPED|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_SYSMENU|WS_THICKFRAME|WS_CAPTION
			dwStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION;
			//常量 WS_OVERLAPPED,
			//常量 WS_MINIMIZEBOX, 有最小化按
			//常量 WS_MAXIMIZEBOX, 有最大化按钮
			//常量 WS_CLIPCHILDREN, 当在父窗口内绘图时,排除子窗口区域
			//常量 WS_SYSMENU, 标题框上带有窗口菜单(须指定WS_CAPTION样式)
			//常量 WS_THICKFRAME , 有可调边框(与WS_SIZEBOX样式相同)
			//常量 WS_CAPTION, 有标题框和边框(和WS_TILED样式相同)
			HWND hWnd = OldCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
			CloseWindowBan(hWnd, TRUE);//禁止窗口CLOASE，防止出事
			return hWnd;
		}

	}
	return OldCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

BOOL HOOK() {
	CreateWindowExW_HOOK.install("user32.dll", "CreateWindowExW", (PROC)MyCreateWindowExW);
	OldCreateWindowExW = (CREATEWINDOWEXW)CreateWindowExW_HOOK.GetOriginalAddress();
	return TRUE;
}



