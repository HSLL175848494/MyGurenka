#ifndef HS_WINIO
#define HS_WINIO

#include<Windows.h>

namespace HSLL
{
	enum WIN_FILE_READ_MODE//�ļ���ȡģʽ
	{
		WIN_FILE_READ_MODE_Read_Write= GENERIC_WRITE| GENERIC_READ,//��д
		WIN_FILE_READ_MODE_Read= GENERIC_READ,//��
		WIN_FILE_READ_MODE_Write= GENERIC_WRITE//д
	};

	enum WIN_FILE_CREATE_FLAG//�ļ���ģʽ
	{
		WIN_FILE_CREATE_FLAG_Create_Always=CREATE_ALWAYS,//ʼ�մ��ļ���Դ�ļ�������ʱ�Զ�����,Դ�ļ�����ʱ�����ļ�����
		WIN_FILE_CREATE_FLAG_Create_New=CREATE_NEW,//��ӦĿ¼���´���һ���ļ�
		WIN_FILE_CREATE_FLAG_Open_Existing=OPEN_EXISTING,//����ļ����ڴ��ļ����߷��ش���
		WIN_FILE_CREATE_FLAG_Open_Truncate_Existing=TRUNCATE_EXISTING,//����ļ�����,���ļ�������ļ�����
		WIN_FILE_CREATE_FLAG_Open_Always=OPEN_ALWAYS//ʼ�մ����ļ���Դ�ļ�������ʱ�Զ�����
	};

	enum WIN_FILE_SHARE_MODE//�ļ�g����ģʽ
	{
		WIN_FILE_SHARE_MODE_Prevent=0,//�޹���
		WIN_FILE_SHARE_MODE_Read_Write= FILE_SHARE_READ|FILE_SHARE_WRITE,//�����д
		WIN_FILE_SHARE_MODE_Read= FILE_SHARE_READ,//�����ȡ
		WIN_FILE_SHARE_MODE_Write=FILE_SHARE_WRITE//����д��
	};

	enum WIN_FILE_POINTER_MOVE_METHOD//ָ���ƶ�ģʽ
	{
		WIN_FILE_MOVE_METHOD_File_Begin=FILE_BEGIN,
		WIN_FILE_MOVE_METHOD_File_Current=FILE_CURRENT,
		WIN_FILE_MOVE_METHOD_File_End=FILE_END
	};


	class WinFile
	{
		HANDLE hFile;//�ļ����

		DWORD State;//״̬

	public:

		//���캯��
		WinFile();

		//�ر��ļ�(�˺�ɴ��´��ļ�)
		BOOL CloseFile();

		//���ļ�
		BOOL OpenFile(LPCSTR FileName, WIN_FILE_READ_MODE WIN_FILE_READ_MODE_, WIN_FILE_CREATE_FLAG WIN_FILE_CREATE_MODE_, WIN_FILE_SHARE_MODE WIN_FILE_SHARE_MODE_);

		//��ȡ����
		BOOL Read(DWORD Size,LPVOID rBuffer,LPDWORD rTrueReadSize);

		//д������
		BOOL Write(DWORD Size,LPVOID Buffer, LPDWORD rTrueWriteSize);

		//�����ļ�ָ��
		BOOL SetFilePointer(LONG Distance, WIN_FILE_POINTER_MOVE_METHOD WIN_FILE_POINTER_MOVE_METHOD_);

		//��ȡ���һ�δ����ַ���(�����FreeErrorString���������ַ���ռ�õ��ڴ�)
		LPSTR GetLastErrorString();

		//�ͷ��ڴ�
		BOOL FreeErrorString(LPSTR ErrorString);

		//��������
		~WinFile();	
	};
}

#endif // !HS_WINIO