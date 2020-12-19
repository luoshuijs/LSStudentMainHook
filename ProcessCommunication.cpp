#include "pch.h"
#include "ProcessCommunication.h"
#include "strsafe.h"
#include "atlstr.h"

WNDPROC OrigEditProc = NULL;
PROC CallFun = NULL;
HWND EnumChildProchwnd = 0;

LRESULT APIENTRY EditSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam);

//创建并打开通信窗口
BOOL ProcessCommunication::open(PROC callFun, int Port)
{
	WNDCLASSEX WndClass;

	//Str处理
	CString StrPort;
	CString MyCommunicationWindowsName;
	StrPort.Format(_T("%d"), Port);
	MyCommunicationWindowsName = _T(CommunicationWindowsName) + StrPort;

	if (IsWindow(PChWnd) && CallFun != NULL) {
		return FALSE;
	}
	//只能创建一个通信窗口
	if (OrigEditProc != NULL) {
		return FALSE;
	}
	WndClass.lpszClassName = MyCommunicationWindowsName;
	WndClass.lpfnWndProc = (WNDPROC)MainWndProc;
	WndClass.hInstance = GetModuleHandle(0);
	WndClass.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;
	//注册窗口
	if (!RegisterClassEx(&WndClass)) {
		return FALSE;
	}
	//创建窗口
	PChWnd = CreateWindowEx(0, WndClass.lpszClassName, L"", WS_OVERLAPPED, 0, 0, 0, 0, 0, 0, 0, 0);//创建窗口
	if (!PChWnd)
	{
		return FALSE;
	}
	MoveWindow(PChWnd, -500, -500, 2, 2, true);//移动窗口，达到隐藏效果
	OrigEditProc = (WNDPROC)SetWindowLong(PChWnd, GWL_WNDPROC, (LONG)EditSubclassProc);//设置新的处理函数
	CallFun = callFun;
	return TRUE;
}

HWND ProcessCommunication::search(int Port) {
	//Str处理
	CString StrPort;
	CString MyCommunicationWindowsName;
	StrPort.Format(_T("%d"), Port);
	MyCommunicationWindowsName = _T(CommunicationWindowsName) + StrPort;
	EnumChildProchwnd = 0;
	EnumChildWindows(NULL, EnumChildProc, Port);
	if (EnumChildProchwnd != 0)
	{
		return EnumChildProchwnd;
	}
	return 0;
}

HWND ProcessCommunication::searchold(int Port) {

	//Str处理
	CString StrPort;
	CString MyCommunicationWindowsName;
	StrPort.Format(_T("%d"), Port);
	MyCommunicationWindowsName = _T(CommunicationWindowsName) + StrPort;
	LPWSTR lpClassName = NULL;

	HWND hWnd = GetWindow(GetDesktopWindow(), GW_CHILD);
	//The EnumChildWindows function is more reliable than calling GetWindow in a loop.
	//An application that calls GetWindow to perform this task risks being caught in
	//an infinite loop or referencing a handle to a window that has been destroyed.
	while (hWnd != NULL) {
		if (hWnd != PChWnd) {
			GetClassName(hWnd, lpClassName, 255);
			if (wcscmp(MyCommunicationWindowsName,lpClassName))
			{
				return hWnd;
			}

		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	return 0;
}

BOOL ProcessCommunication::close() {
	PostMessage(PChWnd, WM_CLOSE, 0, 0);
	//通过IsWindow判断窗口是否关闭（窗口关闭后句柄会无效）
	if ( IsWindow(PChWnd) != TRUE)
	{
		PChWnd = 0;
		CallFun = NULL;
		OrigEditProc = NULL;
		return TRUE;
	}
	return FALSE;
}

// Subclass procedure 
LRESULT APIENTRY EditSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PCOPYDATASTRUCT pMyCDS;
	//判断消息类别
	if (uMsg == WM_COPYDATA) {
		//获取数据
		pMyCDS = (PCOPYDATASTRUCT)lParam;
		//pMyCDS->lpData
		if (CallFun != NULL) {
			CallWindowProc((WNDPROC)CallFun, 0, 0, 0, 0);
		}
	}
	if (uMsg == WM_GETDLGCODE)
		return DLGC_WANTALLKEYS;

	return CallWindowProc(OrigEditProc, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

BOOL CALLBACK EnumChildProc(_In_ HWND hwnd,_In_ LPARAM lParam) {
	LPWSTR lpClassName = NULL;

	//Str处理
	CString StrPort;
	CString MCommunicationWindowsName;
	StrPort.Format(_T("%d"), lParam);
	MCommunicationWindowsName = _T(CommunicationWindowsName) + StrPort;

	GetClassName(hwnd, lpClassName, 255);
	if (wcscmp(lpClassName, MCommunicationWindowsName))
	{
		EnumChildProchwnd = hwnd;
		return FALSE;//结束回调
	}
	return TRUE;
}