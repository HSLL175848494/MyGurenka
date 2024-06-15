#pragma once
#include<Windows.h>
#include<string>

WCHAR* Utf8_To_Wide(const char* str)//utf8数据转宽字节
{
	WCHAR* wide;
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);//获取str所需要宽字节空间的大小
	wide = new WCHAR[i + 1];//申请空间
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wide, i);//将str中的字符装入宽字节数组wide
	return wide;//返回
}


bool IsHalfWidthKatakana(wchar_t ch) 
{
	// 半角片假名字符范围
	return (ch >= 0xFF61 && ch <= 0xFF9F);
}

WCHAR* ConvertSelectedHalfWidthKatakanaToFullWidthHiragana(const wchar_t* input)
{
	// 首先计算输入字符串的长度
	int length = wcslen(input);
	WCHAR* output = new WCHAR[length + 1];  // 为输出字符串分配足够的空间

	for (int i = 0; i < length; ++i)
	{
		if (IsHalfWidthKatakana(input[i]))
		{
			// 对于半角片假名，使用LCMapStringW转换为全角平假名
			::LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_FULLWIDTH | LCMAP_HIRAGANA, &input[i], 1, &output[i], 1);
		}
		else 
		{
			// 其他字符直接复制
			output[i] = input[i];
		}
	}
	output[length] = L'\0';  // 确保输出字符串是以null终止的
	return output;
}

char* TransCoding(const char* str, DWORD from_format, DWORD to_Format) 
{
	int len = MultiByteToWideChar(from_format, 0, str, -1, NULL, 0);
	WCHAR* wide = new WCHAR[len]();
	MultiByteToWideChar(from_format, 0, str, -1, wide, len);

	WCHAR* converted = ConvertSelectedHalfWidthKatakanaToFullWidthHiragana(wide);
	delete[] wide;  // 释放原宽字符串

	int newlen = WideCharToMultiByte(to_Format, 0, converted, -1, NULL, 0, NULL, NULL);

	char* ret = new char[newlen]();
	WideCharToMultiByte(to_Format, 0, converted, -1, ret, newlen, NULL, NULL);

	for (int i = 0; i < newlen; i++)
	{
		if (ret[i] == 0x3f)
		ret[i] = 0x2e;
	}

	delete[] converted;  // 释放转换后的宽字符串

	return ret;
}