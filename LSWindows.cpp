#include "pch.h"
#include "LSWindows.h"

BOOL CloseWindowBan(HWND hWnd, BOOL bBan) {
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	if (hMenu == FALSE)
	{
		return FALSE;
	}
	if (bBan == TRUE)
	{
		EnableMenuItem(hMenu, 6, 1025); //MF_DISABLED ？
	}
	else 
	{
		EnableMenuItem(hMenu, 6, MF_BYPOSITION);
	}
	LPRECT lpRect = NULL;
	if (GetWindowRect(hWnd , lpRect) == FALSE) {
		return FALSE;
	}
	//也许应该可以用DrawMenuBar，以后再说。
	if (RedrawWindow(0, lpRect, 0, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW) == FALSE) {
		return FALSE;
	}
	return TRUE;
}
