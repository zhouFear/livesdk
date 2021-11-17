#pragma once
#include <Windows.h>
#include <xstring>
using namespace std;
class common
{
public:
	static void a2w(CHAR* text, size_t length, WCHAR** res);
	static string ws2s(const wstring& srcStr);
};

