#include"WinFile.h"
#include"transcodingHSLL.h"
using namespace HSLL;


int main()
{
	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA("Script//*.bin", &FindData);//打开目录下的所有文件
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CreateDirectoryA("ScriptUnpack", nullptr);
		CreateDirectoryA("ScriptUnpackInI", nullptr);
		do 
		{
			//获取路径
			char FileName[128] = "Script//";
			lstrcatA(FileName, FindData.cFileName);

			//创建文件流
			WinFile FileDealing;
			FileDealing.OpenFile(FileName, WIN_FILE_READ_MODE_Read, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent);

			//跳过标识
			FileDealing.SetFilePointer(8, WIN_FILE_MOVE_METHOD_File_Current);

			//获取所引索引数量
			DWORD IndexSize;
			FileDealing.Read(4, &IndexSize, nullptr);

			//跳过索引表
			FileDealing.SetFilePointer(IndexSize * 4, WIN_FILE_MOVE_METHOD_File_Current);

			//获取代码大小
			DWORD CodeSize;
			FileDealing.Read(4, &CodeSize, nullptr);

			//获取代码
			BYTE* Code = new BYTE[CodeSize];
			FileDealing.Read(CodeSize, Code, nullptr);

			//InI
			{
				char InIPath[128] = "ScriptUnpackInI//";
				lstrcatA(InIPath, FindData.cFileName);
				DWORD Length = strlen(InIPath) - 1;
				InIPath[Length--] = 'i';
				InIPath[Length--] = 'n';
				InIPath[Length] = 'i';
				WinFile FileInI;
				if (!FileInI.OpenFile(InIPath, WIN_FILE_READ_MODE_Write,WIN_FILE_CREATE_FLAG_Open_Truncate_Existing, WIN_FILE_SHARE_MODE_Prevent))
				FileInI.OpenFile(InIPath, WIN_FILE_READ_MODE_Write, WIN_FILE_CREATE_FLAG_Create_Always, WIN_FILE_SHARE_MODE_Prevent);
				DWORD DialogSize = IndexSize - 1;
				FileInI.Write(4, &DialogSize, nullptr);
				FileInI.Write(CodeSize, Code, nullptr);
				FileInI.CloseFile();
				delete[] Code;
			}

			//获取剧本对话大小
			DWORD DialogSize;
			FileDealing.Read(4, &DialogSize, nullptr);

			//跳过空白占位符号
			FileDealing.SetFilePointer(1, WIN_FILE_MOVE_METHOD_File_Current);

			//读取剧本并关闭流对象
			BYTE* Dialogs = new BYTE[DialogSize];
			FileDealing.Read(DialogSize-1, Dialogs, nullptr);
			FileDealing.CloseFile();

			BYTE** StrArray = new BYTE * [IndexSize - 1];

			//获取原始剧本
			{		
				BYTE* pHead = Dialogs;
				BYTE* pEnd = Dialogs;
				DWORD Size = 0;

				while (true)
				{
					if (Size == IndexSize - 1)
						break;

					DWORD Count = 1;

					while (*pEnd)
					{
						pEnd++;
						Count++;
					}

					StrArray[Size] = new BYTE[Count];
					memcpy(StrArray[Size], pHead, Count);
					pHead = ++pEnd;
					Size++;
				}
				delete[] Dialogs;
			}

			//Txt
			{
				char SavePath[128] = "ScriptUnpack//";
				lstrcatA(SavePath, FindData.cFileName);
				DWORD Length = strlen(SavePath) - 1;
				SavePath[Length--] = 't';
				SavePath[Length--] = 'x';
				SavePath[Length] = 't';

				WinFile FileTxT;
				if (!FileTxT.OpenFile(SavePath, WIN_FILE_READ_MODE_Write,WIN_FILE_CREATE_FLAG_Open_Truncate_Existing, WIN_FILE_SHARE_MODE_Prevent))
				FileTxT.OpenFile(SavePath, WIN_FILE_READ_MODE_Write, WIN_FILE_CREATE_FLAG_Create_Always, WIN_FILE_SHARE_MODE_Prevent);

				for (int i = 0; i < IndexSize - 1; i++)
				{
					char* Str = TransCoding((char*)StrArray[i], 932, 936);
					DWORD Length = strlen(Str);
					Str[Length] = '\n';
					FileTxT.Write(Length + 1, Str, nullptr);
					delete[] Str;
					delete[] StrArray[i];
				}
				delete[] StrArray;

				FileTxT.CloseFile();
			}
		} while (FindNextFileA(hFind, &FindData));
		printf("已完成任务,2s后自动关闭窗口");
		Sleep(2000);
	}
	else
	MessageBoxA(nullptr, "请确保待解包剧本已放置于同级目录下的Script目录下,Script目录可自行创建", "提示", 0);

	return 0;
}