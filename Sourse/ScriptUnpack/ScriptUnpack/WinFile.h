#ifndef HS_WINIO
#define HS_WINIO

#include<Windows.h>

namespace HSLL
{
	enum WIN_FILE_READ_MODE//文件读取模式
	{
		WIN_FILE_READ_MODE_Read_Write= GENERIC_WRITE| GENERIC_READ,//读写
		WIN_FILE_READ_MODE_Read= GENERIC_READ,//读
		WIN_FILE_READ_MODE_Write= GENERIC_WRITE//写
	};

	enum WIN_FILE_CREATE_FLAG//文件打开模式
	{
		WIN_FILE_CREATE_FLAG_Create_Always=CREATE_ALWAYS,//始终打开文件。源文件不存在时自动创建,源文件存在时将清文件内容
		WIN_FILE_CREATE_FLAG_Create_New=CREATE_NEW,//对应目录下新创建一个文件
		WIN_FILE_CREATE_FLAG_Open_Existing=OPEN_EXISTING,//如果文件存在打开文件否者返回错误
		WIN_FILE_CREATE_FLAG_Open_Truncate_Existing=TRUNCATE_EXISTING,//如果文件存在,打开文件并清空文件内容
		WIN_FILE_CREATE_FLAG_Open_Always=OPEN_ALWAYS//始终打开文文件。源文件不存在时自动创建
	};

	enum WIN_FILE_SHARE_MODE//文件g共享模式
	{
		WIN_FILE_SHARE_MODE_Prevent=0,//无共享
		WIN_FILE_SHARE_MODE_Read_Write= FILE_SHARE_READ|FILE_SHARE_WRITE,//共享读写
		WIN_FILE_SHARE_MODE_Read= FILE_SHARE_READ,//共享读取
		WIN_FILE_SHARE_MODE_Write=FILE_SHARE_WRITE//共享写入
	};

	enum WIN_FILE_POINTER_MOVE_METHOD//指针移动模式
	{
		WIN_FILE_MOVE_METHOD_File_Begin=FILE_BEGIN,
		WIN_FILE_MOVE_METHOD_File_Current=FILE_CURRENT,
		WIN_FILE_MOVE_METHOD_File_End=FILE_END
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
		BOOL OpenFile(LPCSTR FileName, WIN_FILE_READ_MODE WIN_FILE_READ_MODE_, WIN_FILE_CREATE_FLAG WIN_FILE_CREATE_MODE_, WIN_FILE_SHARE_MODE WIN_FILE_SHARE_MODE_);

		//读取数据
		BOOL Read(DWORD Size,LPVOID rBuffer,LPDWORD rTrueReadSize);

		//写入数据
		BOOL Write(DWORD Size,LPVOID Buffer, LPDWORD rTrueWriteSize);

		//设置文件指针
		BOOL SetFilePointer(LONG Distance, WIN_FILE_POINTER_MOVE_METHOD WIN_FILE_POINTER_MOVE_METHOD_);

		//获取最近一次错误字符串(请调用FreeErrorString来清理返回字符串占用的内存)
		LPSTR GetLastErrorString();

		//释放内存
		BOOL FreeErrorString(LPSTR ErrorString);

		//析构函数
		~WinFile();	
	};
}

#endif // !HS_WINIO