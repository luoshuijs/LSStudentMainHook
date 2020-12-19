#pragma once

#define CommunicationWindowsName "LSCommunication:" //可自定义监听窗口前缀名

//只支持一个窗口通信（恼火）
class ProcessCommunication
{
public:
	//建立一个与外界通信的监听机制,用于DLL&窗口&控制台等程序间的相互通信(使用本类功能必须先调用本命令,成功返回TRUE,失败返回FALSE)
	BOOL open(
		PROC HookHun, //收到消息后自动触发的事件
		int Port      //用于监听数据到达的消息口(正整数标识,不能雷同,建议数值10000以内,数值太大有可能会被进程保护过滤!)
	);
	HWND searchold(int Port);
	HWND search(int Port);
	BOOL close();
private:
	HWND PChWnd;
};
