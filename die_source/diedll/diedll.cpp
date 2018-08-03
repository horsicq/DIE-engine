//#pragma comment(linker, "/export:DIE_reset=_DIE_reset@0")
//#pragma comment(linker, "/export:DIE_scanA=_DIE_scanA@16")
//#pragma comment(linker, "/export:DIE_scanW=_DIE_scanW@16")
//#pragma comment(linker, "/export:DIE_scanExA=_DIE_scanExA@20")
//#pragma comment(linker, "/export:DIE_scanExW=_DIE_scanExW@20")
//#pragma comment(linker, "/export:DIE_versionA=_DIE_versionA@0")
//#pragma comment(linker, "/export:DIE_versionW=_DIE_versionW@0")

//#include "diedll.h"
//#include "_scan.h"
#include "../singlefilescan.h"



bool  WINAPI  DllMain(_In_  HINSTANCE hinstDLL,_In_  DWORD fdwReason,_In_  LPVOID lpvReserved)
{
    return _DllMain(hinstDLL,fdwReason,lpvReserved);
}

extern "C" int __declspec(dllexport) __stdcall DIE_scanExA(char *pszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, char *pszDataBase)
{
    return __DIE_scanExA(pszFileName,pszOutBuffer,nOutBufferSize,nFlags,pszDataBase);
}

extern "C" int __declspec(dllexport) __stdcall DIE_scanExW(wchar_t *pwszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, wchar_t *pwszDataBase)
{
    return __DIE_scanExW(pwszFileName,pszOutBuffer,nOutBufferSize,nFlags,pwszDataBase);
}

extern "C" int __declspec(dllexport) __stdcall DIE_scanA(char *pszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags)
{
    return __DIE_scanA(pszFileName,pszOutBuffer,nOutBufferSize,nFlags);
}

extern "C" int __declspec(dllexport) __stdcall DIE_scanW(wchar_t *pwszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags)
{
    return __DIE_scanW(pwszFileName,pszOutBuffer,nOutBufferSize,nFlags);
}

extern "C" PCHAR __declspec(dllexport) __stdcall DIE_versionA(void)
{
    return __DIE_versionA();
}

extern "C" PWCHAR __declspec(dllexport) __stdcall DIE_versionW(void)
{
    return __DIE_versionW();
}


