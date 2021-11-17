
#ifdef _WINDOWS_ 
#ifdef DLLEXPORTS_SDK
#define LIVEAPI __declspec(dllexport) 
#else
#define LIVEAPI __declspec(dllimport) 
#endif
#endif

