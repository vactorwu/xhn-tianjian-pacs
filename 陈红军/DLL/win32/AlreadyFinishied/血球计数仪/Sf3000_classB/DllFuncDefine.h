#if !defined(__Dll_FUNCTION_DEFINE_FOR_RL7150_H)   // Sentry, use file only if it's not already included.
#define __Dll_FUNCTION_DEFINE_FOR_RL7150_H

#ifdef _MAC
#undef AFX_EXT_API
#undef AFX_EXT_CLASS
#define AFX_EXT_API
#define AFX_EXT_CLASS
#endif

//动态库基本函数！
extern "C" AFX_EXT_API DWORD WINAPI CommMonitor( LPSTR lpData)
extern "C" AFX_EXT_API void WINAPI BeforeSetCommState(DCB *pComDcb)
//_decspec(dllexport)  DWORD WINAPI CommMonitor( LPSTR lpData)
//_decspec(dllexport) void WINAPI BeforeSetCommState(DCB *pComDcb)
#endif      // __Dll_FUNCTION_DEFINE_FOR_RL7150_H sentry.

#undef AFX_DATA
#define AFX_DATA
