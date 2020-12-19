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

	// �޸�IAT
	while (currentIID)
	{
		//ȥIID�е�Name�ֶΣ���ΪIAT��Ӧ��ģ�����������ڴ��ַƫ��������MessageBoxA����ϵͳ����user32.dll��
		DWORD RVAName = currentIID->Name;
		char* VAName = (char*)(pfile + RVAName);
		if (strcmp(VAName, ModuleName))
		{
			// ����������ȡ��IAT�б�
			DWORD RVAIAT = currentIID->FirstThunk;
			PIMAGE_THUNK_DATA pthunk = (PIMAGE_THUNK_DATA)(pfile + RVAIAT);
			//ѭ��IAT������ַ�б��ҵ�������ַ
			while (pthunk->u1.Function)
			{
				DWORD funAddress = pthunk->u1.Function;
				if (funAddress == FunAddress)
				{
					//�޸�IAT
					DWORD oldProtect = NULL;
					VirtualProtect((LPVOID)&pthunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
					pthunk->u1.Function = HookFun;//�޸ĵ�ַ
					OriginalAddress = (FARPROC)&pthunk->u1.Function;//����ԭ��ַ
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
	// �ҵ�IID���ڵ��ڴ��ַ
	//��ȡ��ǰ��ע��DLL�ĳ���ģ���ַ
	pfile = (PBYTE)GetModuleHandle(NULL);
	if (pfile == NULL) {
		return FALSE;
	}
	//ת����DOSͷ
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)pfile;
	//ͨ��DOSͷ��e_lfanew�ҵ�NTͷ
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(pfile + dosHeader->e_lfanew);
	//NTͷ�еĿ�ѡNTͷ���DataDirectory���飬������IID�������ڴ�ƫ�Ƶ�ַ
	IMAGE_DATA_DIRECTORY IATSection = (IMAGE_DATA_DIRECTORY)(ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
	//���ϻ�ַ����ȡ��IID�����ڴ��ַ
	currentIID = PIMAGE_IMPORT_DESCRIPTOR(pfile + IATSection.VirtualAddress);
	return TRUE;
}

FARPROC IATHOOK::GetOriginalAddress() {
	return OriginalAddress;
}
