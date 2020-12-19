// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "dllmain.h"

//内部引用
#include "MyHook.h"
#include "Process.h"

#include "atlstr.h"

BOOL ProcessMain() {
	CString ProcessName;
	GetProcessNameOld(0, &ProcessName);
	if (wcscmp(ProcessName, L"StudentMain.exe")) {
		HOOK();
	}
	return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		ProcessMain();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



