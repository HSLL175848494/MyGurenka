#include"WinApi.h"
#include<stdio.h>
using namespace HSLL;

int main()
{
	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA("ScriptUnpack//*.txt", &FindData);//打开目录下的所有文件
	if (hFind != INVALID_HANDLE_VALUE)
	{
		CreateDirectoryA("ScriptPack", nullptr);
		do
		{
			char FileName[128] = "ScriptUnpack//";
			lstrcatA(FileName, FindData.cFileName);

			BYTE* DataTxT = nullptr; 
			BYTE* DataInI = nullptr;
			DWORD SizeTxT; DWORD SizeInI;

			//打开剧本txt以及其对应的ini文件读取数据
			{
				WinFile FileTxT;
				FileTxT.OpenFile(FileName, WIN_FILE_READ_MODE_Read, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent);
				SizeTxT =FileTxT.GetFileSize();
				DataTxT = new BYTE[SizeTxT];
				FileTxT.ReadFile(SizeTxT, DataTxT, nullptr);
				FileTxT.CloseFile();


				char PathName[128] = "ScriptUnpackInI//";
				lstrcatA(PathName, FindData.cFileName);
				DWORD Length = strlen(PathName) - 1;
				PathName[Length--] = 'i';
				PathName[Length--] = 'n';
				PathName[Length] = 'i';

				WinFile FileInI;
				FileInI.OpenFile(PathName, WIN_FILE_READ_MODE_Read, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent);
				SizeInI = FileInI.GetFileSize();
				DataInI = new BYTE[SizeInI];
				FileInI.ReadFile(SizeInI, DataInI, nullptr);
				FileInI.CloseFile();
			}

			//获得封包路径路径
			memcpy(&FileName[6], "Pack//", 7);
			lstrcatA(FileName, FindData.cFileName);
			DWORD Length = strlen(FileName)-1;
			FileName[Length--] = 'n';
			FileName[Length--] = 'i';
			FileName[Length] = 'b';

			WinFile FileSave;
			if (!FileSave.OpenFile(FileName, WIN_FILE_READ_MODE_Write, WIN_FILE_CREATE_FLAG_Open_Truncate_Existing, WIN_FILE_SHARE_MODE_Prevent))
				FileSave.OpenFile(FileName, WIN_FILE_READ_MODE_Write, WIN_FILE_CREATE_FLAG_Create_New, WIN_FILE_SHARE_MODE_Prevent);

			BYTE* pTxT = DataTxT;

			//写入标识
			FileSave.WriteFile(8, (LPVOID)"ESCR1_00", nullptr);

			DWORD IndexSize = *(DWORD*)DataInI;

			//写入索引数目
			FileSave.WriteFile(4, &IndexSize, nullptr);

			//索引数组
			DWORD* IndexArray = new DWORD[IndexSize];
			IndexArray[0] = 0;
			IndexArray[1] = 1;
			DWORD Offset = 1;

			//填充索引数组
			for (int i = 0; i < IndexSize - 1; i++)
			{
				while (true)
				{
					if (*pTxT == 0xa)
					{
						*pTxT = '\0';
						Offset++; pTxT++;
						break;
					}
					Offset++; pTxT++;		
				}
				IndexArray[i + 2] = Offset;
			}

			//写入索引
			FileSave.WriteFile(IndexSize*4, IndexArray, nullptr);

			//写入脚本大小
			SizeInI -= 4;
			FileSave.WriteFile(4, &SizeInI, nullptr);

			//写入脚本
			FileSave.WriteFile(SizeInI, DataInI + 4, nullptr);

			//写入剧本大小
			SizeTxT += 1;
			FileSave.WriteFile(4, &SizeTxT, nullptr);

			//填充空白
			BYTE Fill = 0;
			FileSave.WriteFile(1, &Fill, nullptr);

			//写入剧本
			FileSave.WriteFile(SizeTxT-1, DataTxT, nullptr);

			FileSave.CloseFile();

		} while (FindNextFileA(hFind, &FindData));
		printf("已完成任务,2s后自动关闭窗口");
		Sleep(2000);
	}
	else
		MessageBoxA(nullptr, "请确保待封装剧本已放置于同级目录下的ScriptUnpack目录下,配置文件已放置于同级的ScriptUnpackInI文件中\n目录可自行创建", "提示", 0);
	return 0;
}