#pragma once
#include <Windows.h>
#include <xstring>
using namespace std;
class common
{
public:
	static WCHAR* a2w(CHAR* text, size_t length);
	static string ws2s(const wstring& srcStr);
};

