#include "common.h"

WCHAR* common::a2w(CHAR* text, size_t length)
{
	int sz = MultiByteToWideChar(CP_UTF8, 0, text, length, NULL, 0);
	WCHAR* res = new WCHAR[sz];
	memset(static_cast<void*>(res), 0, sz*(sizeof(WCHAR)));
	int nRet = MultiByteToWideChar(CP_ACP, 0, text, length, res, sz);

	if (nRet <= 0)
	{
		DWORD dwErr = GetLastError();
		switch (dwErr)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			OutputDebugStringA("ERROR_INSUFFICIENT_BUFFER\n");
			break;
		case ERROR_INVALID_FLAGS:
			OutputDebugStringA("ERROR_INVALID_FLAGS\n");
			break;
		case ERROR_INVALID_PARAMETER:
			OutputDebugStringA("ERROR_INVALID_PARAMETER\n");
			break;
		case ERROR_NO_UNICODE_TRANSLATION:
			OutputDebugStringA("ERROR_NO_UNICODE_TRANSLATION\n");
			break;
		}
	}
	return res;
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
