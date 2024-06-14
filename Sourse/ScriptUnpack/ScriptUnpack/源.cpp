#include"WinFile.h"
#include"transcodingHSLL.h"
using namespace HSLL;


int main()
{
	WIN32_FIND_DATAA FindData;
	HANDLE hFind = FindFirstFileA("Script//*.bin", &FindData);//��Ŀ¼�µ������ļ�
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CreateDirectoryA("ScriptUnpack", nullptr);
		CreateDirectoryA("ScriptUnpackInI", nullptr);
		do 
		{
			//��ȡ·��
			char FileName[128] = "Script//";
			lstrcatA(FileName, FindData.cFileName);

			//�����ļ���
			WinFile FileDealing;
			FileDealing.OpenFile(FileName, WIN_FILE_READ_MODE_Read, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent);

			//������ʶ
			FileDealing.SetFilePointer(8, WIN_FILE_MOVE_METHOD_File_Current);

			//��ȡ������������
			DWORD IndexSize;
			FileDealing.Read(4, &IndexSize, nullptr);

			//����������
			FileDealing.SetFilePointer(IndexSize * 4, WIN_FILE_MOVE_METHOD_File_Current);

			//��ȡ�����С
			DWORD CodeSize;
			FileDealing.Read(4, &CodeSize, nullptr);

			//��ȡ����
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

			//��ȡ�籾�Ի���С
			DWORD DialogSize;
			FileDealing.Read(4, &DialogSize, nullptr);

			//�����հ�ռλ����
			FileDealing.SetFilePointer(1, WIN_FILE_MOVE_METHOD_File_Current);

			//��ȡ�籾���ر�������
			BYTE* Dialogs = new BYTE[DialogSize];
			FileDealing.Read(DialogSize-1, Dialogs, nullptr);
			FileDealing.CloseFile();

			BYTE** StrArray = new BYTE * [IndexSize - 1];

			//��ȡԭʼ�籾
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
		printf("���������,2s���Զ��رմ���");
		Sleep(2000);
	}
	else
	MessageBoxA(nullptr, "��ȷ��������籾�ѷ�����ͬ��Ŀ¼�µ�ScriptĿ¼��,ScriptĿ¼�����д���", "��ʾ", 0);

	return 0;
}