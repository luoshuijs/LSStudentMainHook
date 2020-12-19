#include "pch.h"
#include "Process.h"
#include "tlhelp32.h"


//通进进程ID取得该进程文件名,可能会受到某些傻逼杀毒软件拦截。
//原理：创建指定的进程的所有模块快照，第一个模块就是该运行程序。
//ProcessID为0时变成获取自进程名称。
BOOL GetProcessNameOld(DWORD ProcessID, CString *ProcessName) {
	MODULEENTRY32W pe32;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		pe32.dwSize = sizeof(MODULEENTRY32W);
		if (Module32First(hSnapshot, &pe32) == TRUE) {
			*ProcessName = pe32.szModule;
			return TRUE;
		}

	}
	return FALSE; 
}

//通进进程ID取得该进程文件名
//原理：创建系统中的所有进程的快照，通过枚举进程找到。
BOOL GetProcessName(DWORD ProcessID, CString *ProcessName) {
	PROCESSENTRY32W pe32;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32First(hSnapshot, &pe32) == TRUE) {
			do
			{
				if (pe32.th32ProcessID == ProcessID) {
					*ProcessName = pe32.szExeFile;
					return TRUE;
				}
			} while (Process32Next(hSnapshot, &pe32) == TRUE);
		}

	}
	return FALSE;
}
