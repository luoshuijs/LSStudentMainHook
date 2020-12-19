#pragma once


class IATHOOK
{
public:
	BOOL install(LPCSTR ModuleName, LPCSTR ProcName, PROC HookFun);
	BOOL WriteIAT(LPCSTR ModuleName, DWORD FunAddress, DWORD HookFun);
	BOOL findIID();
	FARPROC GetOriginalAddress();

private:
	PBYTE pfile = NULL;
	PIMAGE_IMPORT_DESCRIPTOR currentIID = NULL;
	FARPROC OriginalAddress;

};
