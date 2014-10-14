#include "dielib.h"
#include "../singlefilescan.h"

int DIE_scanExA(char *pszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, char *pszDataBase)
{
    return __DIE_scanExA(pszFileName,pszOutBuffer,nOutBufferSize,nFlags,pszDataBase);
}

int DIE_scanExW(wchar_t *pwszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, wchar_t *pwszDataBase)
{
    return __DIE_scanExW(pwszFileName,pszOutBuffer,nOutBufferSize,nFlags,pwszDataBase);
}

int DIE_scanA(char *pszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags)
{
    return __DIE_scanA(pszFileName,pszOutBuffer,nOutBufferSize,nFlags);
}

int DIE_scanW(wchar_t *pwszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags)
{
    return __DIE_scanW(pwszFileName,pszOutBuffer,nOutBufferSize,nFlags);
}

char *DIE_versionA()
{
    return __DIE_versionA();
}

wchar_t *DIE_versionW()
{
    return __DIE_versionW();
}


