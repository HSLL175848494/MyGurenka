#include"WinApi.h"
#include<stdio.h>
using namespace HSLL;

int main()
{
	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA("ScriptUnpack//*.txt", &FindData);//��Ŀ¼�µ������ļ�
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

			//�򿪾籾txt�Լ����Ӧ��ini�ļ���ȡ����
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

			//��÷��·��·��
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

			//д���ʶ
			FileSave.WriteFile(8, (LPVOID)"ESCR1_00", nullptr);

			DWORD IndexSize = *(DWORD*)DataInI;

			//д��������Ŀ
			FileSave.WriteFile(4, &IndexSize, nullptr);

			//��������
			DWORD* IndexArray = new DWORD[IndexSize];
			IndexArray[0] = 0;
			IndexArray[1] = 1;
			DWORD Offset = 1;

			//�����������
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

			//д������
			FileSave.WriteFile(IndexSize*4, IndexArray, nullptr);

			//д��ű���С
			SizeInI -= 4;
			FileSave.WriteFile(4, &SizeInI, nullptr);

			//д��ű�
			FileSave.WriteFile(SizeInI, DataInI + 4, nullptr);

			//д��籾��С
			SizeTxT += 1;
			FileSave.WriteFile(4, &SizeTxT, nullptr);

			//���հ�
			BYTE Fill = 0;
			FileSave.WriteFile(1, &Fill, nullptr);

			//д��籾
			FileSave.WriteFile(SizeTxT-1, DataTxT, nullptr);

			FileSave.CloseFile();

		} while (FindNextFileA(hFind, &FindData));
		printf("���������,2s���Զ��رմ���");
		Sleep(2000);
	}
	else
		MessageBoxA(nullptr, "��ȷ������װ�籾�ѷ�����ͬ��Ŀ¼�µ�ScriptUnpackĿ¼��,�����ļ��ѷ�����ͬ����ScriptUnpackInI�ļ���\nĿ¼�����д���", "��ʾ", 0);
	return 0;
}