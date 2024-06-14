#ifndef  HS_WINIO
#define HS_WINIO

#include<Windows.h>
#include <TlHelp32.h>
#include<Psapi.h>
#pragma comment(lib, "Psapi.lib")

namespace HSLL
{

	enum WIN_FILE_READ_MODE//文件读取模式
	{
		WIN_FILE_READ_MODE_Read_Write = GENERIC_WRITE | GENERIC_READ,//读写
		WIN_FILE_READ_MODE_Read = GENERIC_READ,//读
		WIN_FILE_READ_MODE_Write = GENERIC_WRITE//写
	};

	enum WIN_FILE_CREATE_FLAG//文件打开标志
	{
		WIN_FILE_CREATE_FLAG_Create_Always = CREATE_ALWAYS,//始终打开文件。源文件不存在时自动创建,源文件存在时将清文件内容
		WIN_FILE_CREATE_FLAG_Create_New = CREATE_NEW,//对应目录下新创建一个文件
		WIN_FILE_CREATE_FLAG_Open_Existing = OPEN_EXISTING,//如果文件存在打开文件否者返回错误
		WIN_FILE_CREATE_FLAG_Open_Truncate_Existing = TRUNCATE_EXISTING,//如果文件存在,打开文件并清空文件内容
		WIN_FILE_CREATE_FLAG_Open_Always = OPEN_ALWAYS//始终打开文文件。源文件不存在时自动创建
	};

	enum WIN_FILE_SHARE_MODE//文件共享模式
	{
		WIN_FILE_SHARE_MODE_Prevent = 0,//无共享
		WIN_FILE_SHARE_MODE_Read_Write = FILE_SHARE_READ | FILE_SHARE_WRITE,//共享读写
		WIN_FILE_SHARE_MODE_Read = FILE_SHARE_READ,//共享读取
		WIN_FILE_SHARE_MODE_Write = FILE_SHARE_WRITE//共享写入
	};

	enum WIN_FILE_POINTER_MOVE_MODE//指针移动模式
	{
		WIN_FILE_MOVE_MODE_File_Begin = FILE_BEGIN,
		WIN_FILE_MOVE_MODE_File_Current = FILE_CURRENT,
		WIN_FILE_MOVE_MODE_File_End = FILE_END
	};

	enum CATWAYS//字符拼接模式
	{
		CATWAYS_TAIL,
		CATWAYS_HEAD,
	};


	class WinFile
	{
		HANDLE hFile;//文件句柄

		DWORD State;//状态

	public:

		//构造函数
		WinFile();

		//关闭文件(此后可从新打开文件)
		BOOL CloseFile();

		//打开文件
		BOOL OpenFile(LPCSTR FileName, WIN_FILE_READ_MODE ReadMode, WIN_FILE_CREATE_FLAG CreateFlag, WIN_FILE_SHARE_MODE ShareMode);

		//读取数据
		BOOL ReadFile(DWORD Size, LPVOID rBuffer, LPDWORD rTrueReadBytes);

		//写入数据
		BOOL WriteFile(DWORD Size, LPVOID Buffer, LPDWORD rTrueReadBytes);

		//设置文件指针
		BOOL SetFilePointer(LONG Distance, WIN_FILE_POINTER_MOVE_MODE MoveMode);

		//获取最近一次错误字符串(请调用FreeErrorString来清理返回字符串占用的内存)
		LPSTR GetLastErrorString();

		//释放内存
		BOOL FreeErrorString(LPSTR ErrorString);

		//返回文件大小
		DWORD GetFileSize();

		//析构函数
		~WinFile();
	};

#ifdef _WIN32

	typedef unsigned long long ADDRESS; // 64位时
#else

	typedef unsigned long ADDRESS; // 32位时

#endif

	static class WinString
	{
	public:

		static HANDLE hOutput;//标准输出句柄

		//打印字符串到控制台
		static void WinPrintConsole(LPCWSTR Str);

		//打印字符串到控制台
		static void WinPrintConsole(LPCSTR Str);

		//字符串拼接
		static void LstrcatA(LPSTR Str1, LPSTR Str2, CATWAYS Catways);

		//字符串拼接
		static void LstrcatW(LPWSTR Str1, LPWSTR Str2, CATWAYS Catways);

		//去除扩展名
		static void RemoveExtensionA(LPSTR Str);

		//去除扩展名
		static void RemoveExtensionW(LPWSTR Str);

		//更换拓展名(Length为原来拓展名长度,不含 '.')
		static void ResetExtensionA(LPSTR Src, LPCSTR newExtension, BYTE Length);

		//更换拓展名(Length为原来拓展名长度,不含 '.')
		static void ResetExtensionW(LPWSTR Src, LPCWSTR newExtension, BYTE Length);

		//转码(请用完使用delete删除对应字符串)
		static LPSTR TransCoding(LPCSTR Str,DWORD EncodingFrom,DWORD EncodingTo);

		//宽字符转多字节
		static LPSTR WideCharToMultiByte(LPCWSTR Str, DWORD EncodingTo);

		//多字节转宽字符
		static LPWSTR MultiByteToWideChar(LPCSTR Str, DWORD Encoding);

		//静态方法类删除部分函数
		WinString() = delete;
		WinString(const WinString&) = delete;
		WinString operator&=(const WinString&) = delete;
		WinString(WinString&&) = delete;
	};


	static class WinFunction
	{
	private:

		//获取程序进程id
		static  DWORD GetProcIdEx(LPCWSTR Proc);

	public:

		//获取系统函数地址
		static  ADDRESS  GetSystemFuncAddressA(LPCWSTR Module, LPCSTR Func);

		// 获取其他进程某模块(Dll)的载入地址
		static  ADDRESS  GetProcFuncEx(LPCWSTR Proc,LPCWSTR Module);

		//获取其他进程的基址
		static  ADDRESS  GetProcImageBaseEx(LPCWSTR Proc);

		//获取当前进程的基址
		static  ADDRESS  GetProcImageBase();

		WinFunction() = delete;
		WinFunction(const WinFunction&) = delete;
		WinFunction operator&=(const WinFunction&) = delete;
	};
}

#endif // !HS_WINIO