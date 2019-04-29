#ifndef DIELIB_H
#define DIELIB_H

#include <QtGlobal>

// flags
#define DIE_SHOWERRORS              0x00000001
#define DIE_SHOWOPTIONS             0x00000002
#define DIE_SHOWVERSION             0x00000004
#define DIE_SHOWENTROPY             0x00000008
#define DIE_SINGLELINEOUTPUT        0x00000010
#define DIE_SHOWFILEFORMATONCE      0x00000020

//#ifdef Q_OS_WIN
//typedef unsigned short wchar_t;
//#endif

#ifdef __cplusplus
extern "C" {
#endif
int  DIE_scanA(char *pszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags);
int  DIE_scanW(wchar_t *pwszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags);
int  DIE_scanExA(char *pszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags,char *pszDataBase);
int  DIE_scanExW(wchar_t *pwszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags,wchar_t *pwszDataBase);
char* DIE_versionA();
wchar_t* DIE_versionW();

#ifdef UNICODE
#define DIE_scan DIE_scanW
#define DIE_scanEx DIE_scanExW
#define DIE_version DIE_versionW
#else
#define DIE_scan DIE_scanA
#define DIE_scanEx DIE_scanExA
#define DIE_version DIE_versionA
#endif

#ifdef __cplusplus
}
#endif

#endif // DIELIB_H
