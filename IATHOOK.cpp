#include "pch.h"
#include "IATHOOK.h"
#include "shlwapi.h"

BOOL IATHOOK::install(LPCSTR ModuleName, LPCSTR ProcName, PROC HookFun) {
	HMODULE ModuleHandle = GetModuleHandleA(ModuleName);

	if (ModuleHandle == NULL) {
		ModuleHandle = LoadLibraryA(ModuleName);
	}
	if (ModuleHandle == NULL) {
		return FALSE;
	}
	FARPROC ProcAddress = GetProcAddress(ModuleHandle, ProcName);
	if (ProcAddress == NULL) {
		return FALSE;
	}
	BOOL status = IATHOOK::WriteIAT(ModuleName, (DWORD)ProcAddress, (DWORD)HookFun);
	return status;

}

BOOL IATHOOK::WriteIAT(LPCSTR ModuleName, DWORD FunAddress, DWORD HookFun) {

	if (currentIID == NULL) {
		if (IATHOOK::findIID() == FALSE) {
			return FALSE;
		}
	}

	// 修改IAT
	while (currentIID)
	{
		//去IID中的Name字段，其为IAT对应的模块名称虚拟内存地址偏移量，如MessageBoxA是在系统函数user32.dll中
		DWORD RVAName = currentIID->Name;
		char* VAName = (char*)(pfile + RVAName);
		if (strcmp(VAName, ModuleName))
		{
			// 符合条件，取出IAT列表
			DWORD RVAIAT = currentIID->FirstThunk;
			PIMAGE_THUNK_DATA pthunk = (PIMAGE_THUNK_DATA)(pfile + RVAIAT);
			//循环IAT函数地址列表，找到函数地址
			while (pthunk->u1.Function)
			{
				DWORD funAddress = pthunk->u1.Function;
				if (funAddress == FunAddress)
				{
					//修改IAT
					DWORD oldProtect = NULL;
					VirtualProtect((LPVOID)&pthunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
					pthunk->u1.Function = HookFun;//修改地址
					OriginalAddress = (FARPROC)&pthunk->u1.Function;//保存原地址
					VirtualProtect((LPVOID)&pthunk->u1.Function, 4, oldProtect, &oldProtect);
					break;
				}
				pthunk++;
			}
			return TRUE;
		}
		currentIID++;
	}
	return FALSE;

}

BOOL IATHOOK::findIID() {
	// 找到IID所在的内存地址
	//获取当前被注入DLL的程序模块基址
	pfile = (PBYTE)GetModuleHandle(NULL);
	if (pfile == NULL) {
		return FALSE;
	}
	//转换成DOS头
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)pfile;
	//通过DOS头的e_lfanew找到NT头
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(pfile + dosHeader->e_lfanew);
	//NT头中的可选NT头里的DataDirectory数组，记载着IID的虚拟内存偏移地址
	IMAGE_DATA_DIRECTORY IATSection = (IMAGE_DATA_DIRECTORY)(ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
	//加上基址，获取到IID虚拟内存地址
	currentIID = PIMAGE_IMPORT_DESCRIPTOR(pfile + IATSection.VirtualAddress);
	return TRUE;
}

FARPROC IATHOOK::GetOriginalAddress() {
	return OriginalAddress;
}
