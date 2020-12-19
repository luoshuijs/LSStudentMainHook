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
			//���� WS_OVERLAPPED,
			//���� WS_MINIMIZEBOX, ����С����
			//���� WS_MAXIMIZEBOX, ����󻯰�ť
			//���� WS_CLIPCHILDREN, ���ڸ������ڻ�ͼʱ,�ų��Ӵ�������
			//���� WS_SYSMENU, ������ϴ��д��ڲ˵�(��ָ��WS_CAPTION��ʽ)
			//���� WS_THICKFRAME , �пɵ��߿�(��WS_SIZEBOX��ʽ��ͬ)
			//���� WS_CAPTION, �б����ͱ߿�(��WS_TILED��ʽ��ͬ)
			HWND hWnd = OldCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
			CloseWindowBan(hWnd, TRUE);//��ֹ����CLOASE����ֹ����
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



