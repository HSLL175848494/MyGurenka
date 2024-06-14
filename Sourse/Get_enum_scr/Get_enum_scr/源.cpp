#include"WinApi.h"

using namespace HSLL;

struct ScriptInfo//剧本信息
{
	char SriptName[32];//剧本的名字
	DWORD SerialNumber;//剧本序号
	DWORD SceneNumber;//剧本为第SceneNumber幕
	DWORD ifEnd_Of_A_Page;//是否为剧本某一篇章的结束 如a篇，b篇,c篇
	DWORD ifStart_Of_A_Scene;//是否剧本某一幕的开始
	DWORD SerialNumberRepeat;//重复信息SerialNumber
	char SceneNumbering[16];//剧本对"幕"的编号如 1-01，1-02
	char SceneTitile[32];//某一幕的标题 如：猫と少年
	char Overview[256];//某一幕的概述，讲述了那些内容
};

struct NameInfo//名称表
{
	char CharacterName[64];//人名
	BYTE Othor[36];//含有人物id以及其他描述
};

struct VarInfo//变数
{
	char VarName[32];//变数名
	DWORD SerialNumber;//变数序列号
};

struct SceneInfo//场景回想
{
	DWORD Unknown;//未知数字
	char Overview[64];//场景内容概述
	BYTE Unknown2[32];//未知内容
	DWORD Unknown3;//未知数字
};

struct SegmentHead
{
	DWORD Id;//段id
	DWORD Offset;//一个段偏移
};

struct SegmentScriptInfo
{
	SegmentHead Head;//段头
	ScriptInfo* Info;//剧本信息（第一个始终是0x20填充的空info）
};

struct SegmentNameInfo
{
	SegmentHead Head;//段头
	NameInfo* Info;//名称信息（第一个始终是0x20填充的空info）
};

struct SegmentVarInfo
{
	SegmentHead Head;//段头
	VarInfo* Info;//变量信息（第一个始终是0x20填充的空info）
};

struct SegmentSceneInfo
{
	SegmentHead Head;//段头
	SceneInfo* Info;//回想信息（第一个始终是0x20填充的空info）
};

struct Enum_Scr//enum_scr.bin文件格式
{
	BYTE Signature[8];//4C49535454790100(LISTTY'\01''\0')
	SegmentScriptInfo Scripts;
	SegmentNameInfo Names;
	SegmentVarInfo Vars;
	SegmentSceneInfo Scenes;
};

void WriteString(WinFile* pFile,char* Str)
{   
	WCHAR* wStr=WinString::MultiByteToWideChar(Str,932);
	Str=WinString::WideCharToMultiByte(wStr,936);
	delete[] wStr;
	DWORD Length = lstrlenA(Str);
	Str[Length] = '\n';
	pFile->WriteFile(Length + 1, Str, nullptr);
	delete[] Str;
}

int main()
{
	//打开文件
	WinFile Pack;
	if (!Pack.OpenFile("enum_scr.bin", WIN_FILE_READ_MODE_Read, WIN_FILE_CREATE_FLAG_Open_Existing, WIN_FILE_SHARE_MODE_Prevent))
	{
		WinString::WinPrintConsole("当前目录下不存在enum_scr.bin 5s后窗口自动关闭");
		Sleep(5000);
		return -1;
	}

	//读取数据头
	Enum_Scr Data;
	Pack.ReadFile(8, Data.Signature, nullptr);

	//读取剧本段
	Pack.ReadFile(8, &Data.Scripts.Head, nullptr);
	Data.Scripts.Info = (ScriptInfo*)(new BYTE[Data.Scripts.Head.Offset]);
	Pack.ReadFile(Data.Scripts.Head.Offset, Data.Scripts.Info,nullptr);

	//读取人名段
	Pack.ReadFile(8, &Data.Names.Head, nullptr);
	Data.Names.Info = (NameInfo*)(new BYTE[Data.Names.Head.Offset]);
	Pack.ReadFile(Data.Names.Head.Offset, Data.Names.Info, nullptr);

	//读取变数段
	Pack.ReadFile(8, &Data.Vars.Head, nullptr);
	Data.Vars.Info = (VarInfo*)(new BYTE[Data.Vars.Head.Offset]);
	Pack.ReadFile(Data.Vars.Head.Offset, Data.Vars.Info, nullptr);

	//读取回想段
	Pack.ReadFile(8, &Data.Scenes.Head, nullptr);
	Data.Scenes.Info = (SceneInfo*)(new BYTE[Data.Scenes.Head.Offset]);
	Pack.ReadFile(Data.Scenes.Head.Offset, Data.Scenes.Info, nullptr);
	Pack.CloseFile();

	//保存原始指针
	BYTE* p1 = (BYTE*)Data.Scripts.Info;
	BYTE* p2 = (BYTE*)Data.Names.Info;
	BYTE* p3 = (BYTE*)Data.Vars.Info;
	BYTE* p4 = (BYTE*)Data.Scenes.Info;

	//跳过空白信息
	Data.Scripts.Info++;Data.Names.Info++;Data.Scenes.Info++;

	//打开解包文件
	WinFile Unpack;
	Unpack.OpenFile("unpack.txt", WIN_FILE_READ_MODE_Write,WIN_FILE_CREATE_FLAG_Open_Always, WIN_FILE_SHARE_MODE_Prevent);

	//计算数据数量
	DWORD SizeScripts = Data.Scripts.Head.Offset / sizeof(ScriptInfo) - 1;
	DWORD SizeNames = Data.Names.Head.Offset / sizeof(NameInfo) - 1;
	DWORD SizeScenes = Data.Scenes.Head.Offset / sizeof(SceneInfo) - 1;

	//写入字符串
	for (int i = 0; i < SizeScripts; i++)
	{
		WriteString(&Unpack, Data.Scripts.Info->SceneTitile);
		WriteString(&Unpack, Data.Scripts.Info->Overview);
		Data.Scripts.Info++;
	}

	for (int i = 0; i < SizeNames; i++)
	{
		WriteString(&Unpack, Data.Names.Info->CharacterName);
		Data.Names.Info++;
	}

	for (int i = 0; i < SizeScenes; i++)
	{
		WriteString(&Unpack, Data.Scenes.Info->Overview);
		Data.Scenes.Info++;
	}

	Unpack.CloseFile();

	delete[] p1; delete[] p2; delete[] p3; delete[] p4;

	WinString::WinPrintConsole("已将枚举值提取到unpack.txt 3s后窗口自动关闭");
	Sleep(3000);
	return 0;
}