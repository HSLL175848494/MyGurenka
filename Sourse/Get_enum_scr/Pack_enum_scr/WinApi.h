#ifndef  HS_WINIO
#define HS_WINIO

#include<Windows.h>
#include <TlHelp32.h>
#include<Psapi.h>
#pragma comment(lib, "Psapi.lib")

namespace HSLL
{

	enum WIN_FILE_READ_MODE//�ļ���ȡģʽ
	{
		WIN_FILE_READ_MODE_Read_Write = GENERIC_WRITE | GENERIC_READ,//��д
		WIN_FILE_READ_MODE_Read = GENERIC_READ,//��
		WIN_FILE_READ_MODE_Write = GENERIC_WRITE//д
	};

	enum WIN_FILE_CREATE_FLAG//�ļ��򿪱�־
	{
		WIN_FILE_CREATE_FLAG_Create_Always = CREATE_ALWAYS,//ʼ�մ��ļ���Դ�ļ�������ʱ�Զ�����,Դ�ļ�����ʱ�����ļ�����
		WIN_FILE_CREATE_FLAG_Create_New = CREATE_NEW,//��ӦĿ¼���´���һ���ļ�
		WIN_FILE_CREATE_FLAG_Open_Existing = OPEN_EXISTING,//����ļ����ڴ��ļ����߷��ش���
		WIN_FILE_CREATE_FLAG_Open_Truncate_Existing = TRUNCATE_EXISTING,//����ļ�����,���ļ�������ļ�����
		WIN_FILE_CREATE_FLAG_Open_Always = OPEN_ALWAYS//ʼ�մ����ļ���Դ�ļ�������ʱ�Զ�����
	};

	enum WIN_FILE_SHARE_MODE//�ļ�����ģʽ
	{
		WIN_FILE_SHARE_MODE_Prevent = 0,//�޹���
		WIN_FILE_SHARE_MODE_Read_Write = FILE_SHARE_READ | FILE_SHARE_WRITE,//�����д
		WIN_FILE_SHARE_MODE_Read = FILE_SHARE_READ,//�����ȡ
		WIN_FILE_SHARE_MODE_Write = FILE_SHARE_WRITE//����д��
	};

	enum WIN_FILE_POINTER_MOVE_MODE//ָ���ƶ�ģʽ
	{
		WIN_FILE_MOVE_MODE_File_Begin = FILE_BEGIN,
		WIN_FILE_MOVE_MODE_File_Current = FILE_CURRENT,
		WIN_FILE_MOVE_MODE_File_End = FILE_END
	};

	enum CATWAYS//�ַ�ƴ��ģʽ
	{
		CATWAYS_TAIL,
		CATWAYS_HEAD,
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
		BOOL OpenFile(LPCSTR FileName, WIN_FILE_READ_MODE ReadMode, WIN_FILE_CREATE_FLAG CreateFlag, WIN_FILE_SHARE_MODE ShareMode);

		//��ȡ����
		BOOL ReadFile(DWORD Size, LPVOID rBuffer, LPDWORD rTrueReadBytes);

		//д������
		BOOL WriteFile(DWORD Size, LPVOID Buffer, LPDWORD rTrueReadBytes);

		//�����ļ�ָ��
		BOOL SetFilePointer(LONG Distance, WIN_FILE_POINTER_MOVE_MODE MoveMode);

		//��ȡ���һ�δ����ַ���(�����FreeErrorString���������ַ���ռ�õ��ڴ�)
		LPSTR GetLastErrorString();

		//�ͷ��ڴ�
		BOOL FreeErrorString(LPSTR ErrorString);

		//�����ļ���С
		DWORD GetFileSize();

		//��������
		~WinFile();
	};

#ifdef _WIN32

	typedef unsigned long long ADDRESS; // 64λʱ
#else

	typedef unsigned long ADDRESS; // 32λʱ

#endif

	static class WinString
	{
	public:

		static HANDLE hOutput;//��׼������

		//��ӡ�ַ���������̨
		static void WinPrintConsole(LPCWSTR Str);

		//��ӡ�ַ���������̨
		static void WinPrintConsole(LPCSTR Str);

		//�ַ���ƴ��
		static void LstrcatA(LPSTR Str1, LPSTR Str2, CATWAYS Catways);

		//�ַ���ƴ��
		static void LstrcatW(LPWSTR Str1, LPWSTR Str2, CATWAYS Catways);

		//ȥ����չ��
		static void RemoveExtensionA(LPSTR Str);

		//ȥ����չ��
		static void RemoveExtensionW(LPWSTR Str);

		//������չ��(LengthΪԭ����չ������,���� '.')
		static void ResetExtensionA(LPSTR Src, LPCSTR newExtension, BYTE Length);

		//������չ��(LengthΪԭ����չ������,���� '.')
		static void ResetExtensionW(LPWSTR Src, LPCWSTR newExtension, BYTE Length);

		//ת��(������ʹ��deleteɾ����Ӧ�ַ���)
		static LPSTR TransCoding(LPCSTR Str,DWORD EncodingFrom,DWORD EncodingTo);

		//���ַ�ת���ֽ�
		static LPSTR WideCharToMultiByte(LPCWSTR Str, DWORD EncodingTo);

		//���ֽ�ת���ַ�
		static LPWSTR MultiByteToWideChar(LPCSTR Str, DWORD Encoding);

		//��̬������ɾ�����ֺ���
		WinString() = delete;
		WinString(const WinString&) = delete;
		WinString operator&=(const WinString&) = delete;
		WinString(WinString&&) = delete;
	};


	static class WinFunction
	{
	private:

		//��ȡ�������id
		static  DWORD GetProcIdEx(LPCWSTR Proc);

	public:

		//��ȡϵͳ������ַ
		static  ADDRESS  GetSystemFuncAddressA(LPCWSTR Module, LPCSTR Func);

		// ��ȡ��������ĳģ��(Dll)�������ַ
		static  ADDRESS  GetProcFuncEx(LPCWSTR Proc,LPCWSTR Module);

		//��ȡ�������̵Ļ�ַ
		static  ADDRESS  GetProcImageBaseEx(LPCWSTR Proc);

		//��ȡ��ǰ���̵Ļ�ַ
		static  ADDRESS  GetProcImageBase();

		WinFunction() = delete;
		WinFunction(const WinFunction&) = delete;
		WinFunction operator&=(const WinFunction&) = delete;
	};
}

#endif // !HS_WINIO