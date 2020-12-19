#pragma once

//公开函数


//控制窗口关闭按钮,(允许/禁止)关闭窗口 (成功返回TRUE,失败返回FALSE)
BOOL BanCloseWindow(
	HWND hWnd, //欲禁止或解除禁止的窗口句柄
	BOOL bBan  //是否禁止关闭
);

class LSWindows
{
};

