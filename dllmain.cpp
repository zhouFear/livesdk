#include <Windows.h>

BOOL APIENTRY DllMain(void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved) {
	switch (_Reason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}