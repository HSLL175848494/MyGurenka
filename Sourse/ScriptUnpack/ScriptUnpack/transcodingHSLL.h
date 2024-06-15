#pragma once
#include<Windows.h>
#include<string>

WCHAR* Utf8_To_Wide(const char* str)//utf8����ת���ֽ�
{
	WCHAR* wide;
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);//��ȡstr����Ҫ���ֽڿռ�Ĵ�С
	wide = new WCHAR[i + 1];//����ռ�
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wide, i);//��str�е��ַ�װ����ֽ�����wide
	return wide;//����
}


bool IsHalfWidthKatakana(wchar_t ch) 
{
	// ���Ƭ�����ַ���Χ
	return (ch >= 0xFF61 && ch <= 0xFF9F);
}

WCHAR* ConvertSelectedHalfWidthKatakanaToFullWidthHiragana(const wchar_t* input)
{
	// ���ȼ��������ַ����ĳ���
	int length = wcslen(input);
	WCHAR* output = new WCHAR[length + 1];  // Ϊ����ַ��������㹻�Ŀռ�

	for (int i = 0; i < length; ++i)
	{
		if (IsHalfWidthKatakana(input[i]))
		{
			// ���ڰ��Ƭ������ʹ��LCMapStringWת��Ϊȫ��ƽ����
			::LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_FULLWIDTH | LCMAP_HIRAGANA, &input[i], 1, &output[i], 1);
		}
		else 
		{
			// �����ַ�ֱ�Ӹ���
			output[i] = input[i];
		}
	}
	output[length] = L'\0';  // ȷ������ַ�������null��ֹ��
	return output;
}

char* TransCoding(const char* str, DWORD from_format, DWORD to_Format) 
{
	int len = MultiByteToWideChar(from_format, 0, str, -1, NULL, 0);
	WCHAR* wide = new WCHAR[len]();
	MultiByteToWideChar(from_format, 0, str, -1, wide, len);

	WCHAR* converted = ConvertSelectedHalfWidthKatakanaToFullWidthHiragana(wide);
	delete[] wide;  // �ͷ�ԭ���ַ���

	int newlen = WideCharToMultiByte(to_Format, 0, converted, -1, NULL, 0, NULL, NULL);

	char* ret = new char[newlen]();
	WideCharToMultiByte(to_Format, 0, converted, -1, ret, newlen, NULL, NULL);

	for (int i = 0; i < newlen; i++)
	{
		if (ret[i] == 0x3f)
		ret[i] = 0x2e;
	}

	delete[] converted;  // �ͷ�ת����Ŀ��ַ���

	return ret;
}