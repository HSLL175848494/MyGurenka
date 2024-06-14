#include "WinFile.h"

//����
HSLL::WinFile::WinFile() :hFile(INVALID_HANDLE_VALUE), State(0) {}

//�ر��ļ�
BOOL HSLL::WinFile::CloseFile()
{
	if (State & 0x1)
	{
		CloseHandle(hFile);
		State &= 0xffff0000;
		return true;
	}
	return false;
}

//���ļ�
BOOL HSLL::WinFile::OpenFile(LPCSTR FileName, WIN_FILE_READ_MODE WIN_FILE_READ_MODE_, 
	WIN_FILE_CREATE_FLAG WIN_FILE_CREATE_MODE_, WIN_FILE_SHARE_MODE WIN_FILE_SHARE_MODE_)
{
	hFile = CreateFileA(FileName, WIN_FILE_READ_MODE_, WIN_FILE_SHARE_MODE_, nullptr, WIN_FILE_CREATE_MODE_, FILE_ATTRIBUTE_NORMAL, nullptr);

	State = State%0xffff+(GetLastError() << 16);

	if (hFile == INVALID_HANDLE_VALUE)
	return false;

	State |= 0x1;
	return true;
}

//��ȡ����
BOOL HSLL::WinFile::Read(DWORD Size, LPVOID rBuffer, LPDWORD rTrueReadSize)
{
	if (!(State & 0x1))
	return false;

	BOOL Result = ReadFile(hFile, rBuffer, Size, rTrueReadSize, 0);
	State = State % 0xffff + (GetLastError() << 16);
	return Result;
}

//д������
BOOL HSLL::WinFile::Write(DWORD Size, LPVOID Buffer, LPDWORD rTrueWriteSize)
{
	if (!(State & 0x1))
		return false;

	BOOL Result = WriteFile(hFile, Buffer, Size, rTrueWriteSize, 0);
	State = State % 0xffff + (GetLastError() << 16);
	return Result;
}

//�ƶ��ļ�ָ��
BOOL HSLL::WinFile::SetFilePointer(LONG Distance, WIN_FILE_POINTER_MOVE_METHOD WIN_FILE_POINTER_MOVE_METHOD_)
{
	if (!(State & 0x1))
		return false;

	State = State % 0xffff + (GetLastError() << 16);

	if (::SetFilePointer(hFile, Distance, 0, WIN_FILE_POINTER_MOVE_METHOD_) == INVALID_SET_FILE_POINTER)
	return false;
	else
	return true;
}

//��ȡ���һ�δ���
LPSTR HSLL::WinFile::GetLastErrorString()
{
	WORD ErrorCode = State >> 16;

	if (!ErrorCode)
		return 0;

	LPSTR msgBuffer;	
	DWORD msgLength = FormatMessageA(	FORMAT_MESSAGE_ALLOCATE_BUFFER |	FORMAT_MESSAGE_FROM_SYSTEM |	FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,	ErrorCode,	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	(LPSTR)&msgBuffer,	0,	nullptr);
   
	State = State % 0xffff + (GetLastError() << 16);

	return msgBuffer;
}

//�����ڴ�
BOOL HSLL::WinFile::FreeErrorString(LPSTR ErrorString)
{
	BOOL Result=!LocalFree(ErrorString);

	State = State % 0xffff + (GetLastError() << 16);

	return Result;
}

//�����ļ���С
BOOL HSLL::WinFile::GetFileSize()
{
	if (!(State & 0x1))
		return false;

	DWORD Size=::GetFileSize(hFile,nullptr);
	State = State % 0xffff + (GetLastError() << 16);
	return Size;
}

//����
HSLL::WinFile::~WinFile()
{
	if (State & 0x1)
	CloseHandle(hFile);
}