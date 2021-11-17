#include "common.h"

void common::a2w(CHAR* text, size_t length, WCHAR** res)
{
	int sz = MultiByteToWideChar(CP_UTF8, 0, text, length, NULL, 0);
	*res = new WCHAR[sz];
	memset(static_cast<void*>(*res), 0, sz*(sizeof(WCHAR)));
	int nRet = MultiByteToWideChar(CP_ACP, 0, text, length, *res, sz);
}

std::string common::ws2s(const wstring& srcStr)
{
	int sz = WideCharToMultiByte(CP_ACP, 0, srcStr.c_str(), srcStr.length(), NULL, 0, NULL, FALSE);
	char* res = new char[sz];
	memset(static_cast<void*>(res), 0, sz * (sizeof(char)));
	int nRet = WideCharToMultiByte(CP_UTF8, 0, srcStr.c_str(), srcStr.length(), res, sz, NULL, FALSE);
	string dst = res;
	delete[] res;
	return dst;
}
