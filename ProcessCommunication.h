#pragma once

#define CommunicationWindowsName "LSCommunication:" //���Զ����������ǰ׺��

//ֻ֧��һ������ͨ�ţ��ջ�
class ProcessCommunication
{
public:
	//����һ�������ͨ�ŵļ�������,����DLL&����&����̨�ȳ������໥ͨ��(ʹ�ñ��๦�ܱ����ȵ��ñ�����,�ɹ�����TRUE,ʧ�ܷ���FALSE)
	BOOL open(
		PROC HookHun, //�յ���Ϣ���Զ��������¼�
		int Port      //���ڼ������ݵ������Ϣ��(��������ʶ,������ͬ,������ֵ10000����,��ֵ̫���п��ܻᱻ���̱�������!)
	);
	HWND searchold(int Port);
	HWND search(int Port);
	BOOL close();
private:
	HWND PChWnd;
};
