// LSStudentMainHook.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "LSStudentMainHook.h"


// 这是导出变量的一个示例
LSSTUDENTMAINHOOK_API int nLSStudentMainHook=0;

// 这是导出函数的一个示例。
LSSTUDENTMAINHOOK_API int fnLSStudentMainHook(void)
{
    return 0;
}

// 这是已导出类的构造函数。
CLSStudentMainHook::CLSStudentMainHook()
{
    return;
}
