#include <windows.h>

#ifdef PROJECT1_EXPORTS
#define LOADTIME_API  __declspec(dllexport)
#else
#define LOADTIME_API  __declspec(dllimport)
#endif
extern "C" __declspec(dllexport) bool procd3(const char* a, char b);
