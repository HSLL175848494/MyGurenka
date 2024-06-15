#include"WinApi.h"

using namespace HSLL;

struct ScriptInfo//�籾��Ϣ
{
	char SriptName[32];//�籾������
	DWORD SerialNumber;//�籾���
	DWORD SceneNumber;//�籾Ϊ��SceneNumberĻ
	DWORD ifEnd_Of_A_Page;//�Ƿ�Ϊ�籾ĳһƪ�µĽ��� ��aƪ��bƪ,cƪ
	DWORD ifStart_Of_A_Scene;//�Ƿ�籾ĳһĻ�Ŀ�ʼ
	DWORD SerialNumberRepeat;//�ظ���ϢSerialNumber
	char SceneNumbering[16];//�籾��"Ļ"�ı���� 1-01��1-02
	char SceneTitile[32];//ĳһĻ�ı��� �磺è������
	char Overview[256];//ĳһĻ�ĸ�������������Щ����
};

struct NameInfo//���Ʊ�
{
	char CharacterName[64];//����
	BYTE Othor[36];//��������id�Լ���������
};

struct VarInfo//����
{
	char VarName[32];//������
	DWORD SerialNumber;//�������к�
};

struct SceneInfo//��������
{
	DWORD Unknown;//δ֪����
	char Overview[64];//�������ݸ���
	BYTE Unknown2[32];//δ֪����
	DWORD Unknown3;//δ֪����
};

struct SegmentHead
{
	DWORD Id;//��id
	DWORD Offset;//һ����ƫ��
};

struct SegmentScriptInfo
{
	SegmentHead Head;//��ͷ
	ScriptInfo* Info;//�籾��Ϣ����һ��ʼ����0x20���Ŀ�info��
};

struct SegmentNameInfo
{
	SegmentHead Head;//��ͷ
	NameInfo* Info;//������Ϣ����һ��ʼ����0x20���Ŀ�info��
};

struct SegmentVarInfo
{
	SegmentHead Head;//��ͷ
	VarInfo* Info;//������Ϣ����һ��ʼ����0x20���Ŀ�info��
};

struct SegmentSceneInfo
{
	SegmentHead Head;//��ͷ
	SceneInfo* Info;//������Ϣ����һ��ʼ����0x20���Ŀ�info��
};

struct Enum_Scr//enum_scr.bin�ļ���ʽ
{
	BYTE Signature[8];//4C49535454790100(LISTTY'\01''\0')
	SegmentScriptInfo Scripts;
	SegmentNameInfo Names;
	SegmentVarInfo Vars;
	SegmentSceneInfo Scenes;
};

int main()
{
	//���ļ�
	WinFile Pack;
	if (!Pack.OpenFile("enum_scr.bin", WIN_FILE_READ_MODE_Read_Write, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent))
	{
		WinString::WinPrintConsole("��ǰĿ¼�²�����enum_scr.bin 5s�󴰿��Զ��ر�");
		Sleep(5000);
		return -1;
	}

	//�򿪽���ļ�
	WinFile Unpack;
	if (!Unpack.OpenFile("unpack.txt", WIN_FILE_READ_MODE_Read, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent))
	{
		WinString::WinPrintConsole("��ǰĿ¼�²�����unpack.txt 5s�󴰿��Զ��ر�");
		Sleep(5000);
		return -1;
	}

	//��ȡ����ͷ
	Enum_Scr Data;
	Pack.ReadFile(8, Data.Signature, nullptr);

	//��ȡ�籾��
	Pack.ReadFile(8, &Data.Scripts.Head, nullptr);
	Data.Scripts.Info = (ScriptInfo*)(new BYTE[Data.Scripts.Head.Offset]);
	Pack.ReadFile(Data.Scripts.Head.Offset, Data.Scripts.Info, nullptr);

	//��ȡ������
	Pack.ReadFile(8, &Data.Names.Head, nullptr);
	Data.Names.Info = (NameInfo*)(new BYTE[Data.Names.Head.Offset]);
	Pack.ReadFile(Data.Names.Head.Offset, Data.Names.Info, nullptr);

	//��ȡ������
	Pack.ReadFile(8, &Data.Vars.Head, nullptr);
	Data.Vars.Info = (VarInfo*)(new BYTE[Data.Vars.Head.Offset]);
	Pack.ReadFile(Data.Vars.Head.Offset, Data.Vars.Info, nullptr);

	//��ȡ�����
	Pack.ReadFile(8, &Data.Scenes.Head, nullptr);
	Data.Scenes.Info = (SceneInfo*)(new BYTE[Data.Scenes.Head.Offset]);
	Pack.ReadFile(Data.Scenes.Head.Offset, Data.Scenes.Info, nullptr);

	//����ԭʼָ��
	BYTE* p1 = (BYTE*)Data.Scripts.Info;
	BYTE* p2 = (BYTE*)Data.Names.Info;
	BYTE* p3 = (BYTE*)Data.Vars.Info;
	BYTE* p4 = (BYTE*)Data.Scenes.Info;

	//�����հ���Ϣ
	Data.Scripts.Info++; Data.Names.Info++; Data.Scenes.Info++;

	//��ȡ����ļ�
	BYTE* Buffer = new BYTE[Unpack.GetFileSize()];
	Unpack.ReadFile(Unpack.GetFileSize(), Buffer, nullptr);
	Unpack.CloseFile();

	//������������
	DWORD SizeScripts = Data.Scripts.Head.Offset / sizeof(ScriptInfo) - 1;
	DWORD SizeNames = Data.Names.Head.Offset / sizeof(NameInfo) - 1;
	DWORD SizeScenes = Data.Scenes.Head.Offset / sizeof(SceneInfo) - 1;

	//�ַ���ָ��
	BYTE* Ptr = Buffer;
	BYTE* prePtr = Buffer;
	DWORD Count = 0;

	//�����ַ���
	auto UpdateString = [&](char* Str)->void{

		while (true)
		{
			if (*Ptr == '\n')
			{	
				*Ptr = '\0';
				Count++; Ptr++;
				break;
			}
			Count++; Ptr++;
		}
		memcpy(Str, prePtr, Count);
		Count = 0; prePtr = Ptr;
	};

	//����д���ַ���
	for (int i = 0; i < SizeScripts; i++)
	{
		UpdateString(Data.Scripts.Info->SceneTitile);
		UpdateString(Data.Scripts.Info->Overview);
		Data.Scripts.Info++;
	}

	for (int i = 0; i < SizeNames; i++)
	{
		UpdateString(Data.Names.Info->CharacterName);
		Data.Names.Info++;
	}

	for (int i = 0; i < SizeScenes; i++)
	{
		UpdateString(Data.Scenes.Info->Overview);
		Data.Scenes.Info++;
	}

	delete[] Buffer;

	//����д���ļ�
	Pack.SetFilePointer(0,WIN_FILE_MOVE_MODE_File_Begin);

	//д���ļ���ʶ
	Pack.WriteFile(8, Data.Signature, nullptr);

	//д��script��
	Pack.WriteFile(8, &Data.Scripts.Head, nullptr);
	Pack.WriteFile(Data.Scripts.Head.Offset, p1, nullptr);

	//д��name��
	Pack.WriteFile(8, &Data.Names.Head, nullptr);
	Pack.WriteFile(Data.Names.Head.Offset, p2, nullptr);

	//д��var��
	Pack.WriteFile(8, &Data.Vars.Head, nullptr);
	Pack.WriteFile(Data.Vars.Head.Offset, p3, nullptr);

	//д��scene��
	Pack.WriteFile(8, &Data.Scenes.Head, nullptr);
	Pack.WriteFile(Data.Scenes.Head.Offset, p4, nullptr);

	//����
	Pack.CloseFile();
	delete[] p1; delete[] p2; delete[] p3; delete[] p4;

	WinString::WinPrintConsole("�ѳɹ���utf8������ʽ�滻Դ�ı� 3s�󴰿��Զ��ر�");
	Sleep(3000);
	return 0;
}