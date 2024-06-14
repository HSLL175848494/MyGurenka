#include"WinApi.h"


//构造
HSLL::WinFile::WinFile() :hFile(INVALID_HANDLE_VALUE), State(0) {}


//关闭文件
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


//打开文件
BOOL HSLL::WinFile::OpenFile(LPCSTR FileName, WIN_FILE_READ_MODE ReadMode,
	WIN_FILE_CREATE_FLAG CreateFlag, WIN_FILE_SHARE_MODE ShareMode)
{
	hFile = CreateFileA(FileName, ReadMode, ShareMode, nullptr, CreateFlag, FILE_ATTRIBUTE_NORMAL, nullptr);

	State = State % 0xffff + (GetLastError() << 16);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	State |= 0x1;
	return true;
}


//读取数据
BOOL HSLL::WinFile::ReadFile(DWORD Size, LPVOID rBuffer, LPDWORD rTrueReadBytes)
{
	if (!(State & 0x1))
		return false;

	BOOL Result = ::ReadFile(hFile, rBuffer, Size, rTrueReadBytes, 0);
	State = State % 0xffff + (GetLastError() << 16);
	return Result;
}


//写入数据
BOOL HSLL::WinFile::WriteFile(DWORD Size, LPVOID Buffer, LPDWORD rTrueWriteSize)
{
	if (!(State & 0x1))
		return false;

	BOOL Result = ::WriteFile(hFile, Buffer, Size, rTrueWriteSize, 0);
	State = State % 0xffff + (GetLastError() << 16);
	return Result;
}


//移动文件指针
BOOL HSLL::WinFile::SetFilePointer(LONG Distance, WIN_FILE_POINTER_MOVE_MODE MoveMethod)
{
	if (!(State & 0x1))
		return false;

	State = State % 0xffff + (GetLastError() << 16);

	if (::SetFilePointer(hFile, Distance, 0, MoveMethod) == INVALID_SET_FILE_POINTER)
		return false;
	else
		return true;
}


//获取最后一次错误
LPSTR HSLL::WinFile::GetLastErrorString()
{
	WORD ErrorCode = State >> 16;

	if (!ErrorCode)
		return 0;

	LPSTR msgBuffer;
	DWORD msgLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuffer, 0, nullptr);

	State = State % 0xffff + (GetLastError() << 16);

	return msgBuffer;
}


//清理内存
BOOL HSLL::WinFile::FreeErrorString(LPSTR ErrorString)
{
	BOOL Result = !LocalFree(ErrorString);

	State = State % 0xffff + (GetLastError() << 16);

	return Result;
}


//返回文件大小
DWORD HSLL::WinFile::GetFileSize()
{
	if (!(State & 0x1))
		return false;

	DWORD Size = ::GetFileSize(hFile, nullptr);
	State = State % 0xffff + (GetLastError() << 16);
	return Size;
}


//析构
HSLL::WinFile::~WinFile()
{
	if (State & 0x1)
		CloseHandle(hFile);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////WinOutput

//初始化标准输出句柄
HANDLE HSLL::WinString::hOutput = GetStdHandle(STD_OUTPUT_HANDLE);


//打印字符串到控制台
void HSLL::WinString::WinPrintConsole(LPCWSTR Str)
{
	WriteConsoleW(hOutput, Str, wcslen(Str) + 1, nullptr, nullptr);
}


//打印字符串到控制台
void HSLL::WinString::WinPrintConsole(LPCSTR Str)
{
	WriteConsoleA(hOutput, Str, strlen(Str) + 1, nullptr, nullptr);
}


//字符串拼接
void HSLL::WinString::LstrcatA(LPSTR Str1, LPSTR Str2, CATWAYS Catways)
{
	if (Catways == CATWAYS_HEAD)
	{
		DWORD Length = strlen(Str2);
		memmove(Str1 + Length, Str1, strlen(Str1) + 1);
		memmove(Str1, Str2, Length);
	}
	else
		lstrcatA(Str1, Str2);
}


//字符串拼接
void HSLL::WinString::LstrcatW(LPWSTR Str1, LPWSTR Str2, CATWAYS Catways)
{
	if (Catways == CATWAYS_HEAD)
	{
		DWORD Length = wcslen(Str2);
		memmove(Str1 + Length, Str1, (wcslen(Str1) + 1) * sizeof(wchar_t));
		memmove(Str1, Str2, Length * sizeof(wchar_t));
	}
	else
		lstrcatW(Str1, Str2);
}


//去除扩展名
void HSLL::WinString::RemoveExtensionA(LPSTR Str)
{
	DWORD Length = strlen(Str);

	while (Length >= 1 && Str[Length - 1] != '.')
		Length--;

	if (Length != 0)
		Str[Length - 1] = '\0';
}


//去除扩展名
void HSLL::WinString::RemoveExtensionW(LPWSTR Str)
{
	DWORD Length = wcslen(Str);

	while (Length >= 1 && Str[Length - 1] != L'.')
		Length--;

	if (Length != 0)
		Str[Length - 1] = L'\0';
}


//更换拓展名(Length为原来拓展名长度,不含 '.')
void HSLL::WinString::ResetExtensionA(LPSTR Src, LPCSTR newExtension, BYTE Length)
{
	DWORD sLength = strlen(Src);
	DWORD eLength = strlen(newExtension);
	memcpy(Src + sLength - Length, newExtension, eLength + 1);
}


//更换拓展名(Length为原来拓展名长度,不含 '.')
void HSLL::WinString::ResetExtensionW(LPWSTR Src, LPCWSTR newExtension, BYTE Length)
{
	DWORD sLength = wcslen(Src);
	DWORD eLength = wcslen(newExtension);
	memcpy(Src + sLength - Length, newExtension, (eLength + 1) * sizeof(wchar_t));
}


//转码(请用完使用delete删除对应字符串)
LPSTR HSLL::WinString::TransCoding(LPCSTR Str, DWORD EncodingFrom, DWORD EncodingTo)
{
	if (Str == nullptr)
		return nullptr;
	int wideCharLen = ::MultiByteToWideChar(EncodingFrom, 0, Str, -1, NULL, 0);
	if (wideCharLen == 0)
		return nullptr;
	WCHAR* wideCharStr = new WCHAR[wideCharLen];
	::MultiByteToWideChar(EncodingFrom, 0, Str, -1, wideCharStr, wideCharLen);
	int multiByteLen = ::WideCharToMultiByte(EncodingTo, 0, wideCharStr, -1, NULL, 0, NULL, NULL);
	if (multiByteLen == 0)
	{
		delete[] wideCharStr;
		return nullptr;
	}
	LPSTR multiByteStr = new CHAR[multiByteLen];
	::WideCharToMultiByte(EncodingTo, 0, wideCharStr, -1, multiByteStr, multiByteLen, NULL, NULL);
	delete[] wideCharStr;
	return multiByteStr;
}


//宽字符转多字节
LPSTR HSLL::WinString::WideCharToMultiByte(LPCWSTR Str, DWORD EncodingTo)
{
	if (Str == nullptr)
		return nullptr;

	int multiByteLen = ::WideCharToMultiByte(EncodingTo, 0, Str, -1, NULL, 0, NULL, NULL);
	if (multiByteLen == 0)
		return nullptr;

	LPSTR multiByteStr = new CHAR[multiByteLen];
	::WideCharToMultiByte(EncodingTo, 0, Str, -1, multiByteStr, multiByteLen, NULL, NULL);

	return multiByteStr;
}

//多字节转宽字符
LPWSTR HSLL::WinString::MultiByteToWideChar(LPCSTR Str, DWORD Encoding)
{
	if (Str == nullptr)
		return nullptr;

	int wideCharLen = ::MultiByteToWideChar(Encoding, 0, Str, -1, NULL, 0);
	if (wideCharLen == 0)
		return nullptr;

	LPWSTR wideCharStr = new WCHAR[wideCharLen];
	::MultiByteToWideChar(Encoding, 0, Str, -1, wideCharStr, wideCharLen);

	return wideCharStr;
}


// 获取系统函数地址 (ASCII)
HSLL::ADDRESS HSLL::WinFunction::GetSystemFuncAddressA(LPCWSTR Module, LPCSTR Func)
{
	HMODULE hModule = GetModuleHandleW(Module);

	if (hModule == NULL)
	{
		hModule = LoadLibraryW(Module);

		if (hModule == NULL)
			return 0;
	}

	FARPROC funcAddress = GetProcAddress(hModule, Func);

	if (funcAddress == NULL)
		return 0;

	return (HSLL::ADDRESS)funcAddress;
}


// 获取进程ID
DWORD HSLL::WinFunction::GetProcIdEx(LPCWSTR Proc)
{
	DWORD procId = 0;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 遍历进程列表，找到指定进程
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if (lstrcmpW(pe32.szExeFile, Proc) == 0)
			{
				procId = pe32.th32ProcessID;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}

	CloseHandle(hProcessSnap);
	return procId;
}


HSLL::ADDRESS HSLL::WinFunction::GetProcFuncEx(LPCWSTR Proc, LPCWSTR Module)
{
	ADDRESS moduleBaseAddress = 0;
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(PROCESSENTRY32W);
	if (Process32First(hSnapshot, &pe))
	{
		do 
		{
			if (!lstrcmpiW(pe.szExeFile, Proc))
			{
				processId = pe.th32ProcessID;
				break;
			}
		} while (Process32NextW(hSnapshot, &pe));
	}
	CloseHandle(hSnapshot);

	if (processId == 0)
		return 0;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

	if (hSnapshot == INVALID_HANDLE_VALUE) 
		return 0;

	MODULEENTRY32W me;
	me.dwSize = sizeof(MODULEENTRY32W);

	if (Module32FirstW(hSnapshot, &me))
	{
		do 
		{
			if (!lstrcmpiW(me.szModule, Module))
			{
				moduleBaseAddress = (ADDRESS)(me.modBaseAddr);
				break;
			}
		} while (Module32NextW(hSnapshot, &me));
	}

	CloseHandle(hSnapshot);
	return moduleBaseAddress;
}


// 获取其他进程的基址
HSLL::ADDRESS HSLL::WinFunction::GetProcImageBaseEx(LPCWSTR Proc)
{
	DWORD procId = GetProcIdEx(Proc);

	if (procId == 0)
		return 0;

	HANDLE hModuleSnap;
	MODULEENTRY32W me32;

	// 创建模块快照
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId);

	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return 0;

	me32.dwSize = sizeof(MODULEENTRY32W);

	// 获取进程基址
	if (Module32FirstW(hModuleSnap, &me32))
	{
		CloseHandle(hModuleSnap);

		return (ADDRESS)me32.modBaseAddr;
	}

	CloseHandle(hModuleSnap);
	return 0;
}


// 获取当前进程的基址
HSLL::ADDRESS HSLL::WinFunction::GetProcImageBase()
{
	MODULEINFO ModuleInfo = { 0 };
	HMODULE hModule = GetModuleHandleW(nullptr);

	if (hModule == nullptr)
		return 0;

	if (!K32GetModuleInformation(GetCurrentProcess(), hModule, &ModuleInfo, sizeof(MODULEINFO)))
		return 0;

	return (ADDRESS)ModuleInfo.lpBaseOfDll;
}