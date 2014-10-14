#ifndef SINGLEFILESCAN_H
#define SINGLEFILESCAN_H

#include <QObject>
#include <QString>
#include "_options.h"
#include "scan.h"

class SingleFileScan : public QObject
{
Q_OBJECT
public:
    SingleFileScan(QObject *parent = 0);

//    void setOptions(__DIE_OPTIONS *pOptions);
    static void flagsToOptions(unsigned int nFlags,__DIE_OPTIONS *pOptions);
    QString process(QString sFileName);
    static QString firstBytes(QString sFileName,unsigned int nSize);

    static void setDataBase(QString sDataBase);

    static __DIE_OPTIONS options;
public slots:

private slots:
    void appendMessage(QString sMessage);
    void appendSignature(QString sMessage);
    void appendError(QString sMessage);
private:
//    __DIE_OPTIONS *pOptions;
    QString sResult;
};
#ifdef Q_WS_WIN
bool  WINAPI  _DllMain(_In_  HINSTANCE hinstDLL,_In_  DWORD fdwReason,_In_  LPVOID lpvReserved);
#endif
int __DIE_scan(QString szFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags,QString sDataBase);
int __DIE_scanExA(char *pszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, char *pszDataBase);
int __DIE_scanExW(wchar_t *pwszFileName, char *pszOutBuffer, int nOutBufferSize, unsigned int nFlags, wchar_t *pwszDataBase);
int __DIE_scanA(char *pszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags);
int __DIE_scanW(wchar_t *pwszFileName,char *pszOutBuffer,int nOutBufferSize,unsigned int nFlags);
char *__DIE_versionA(void);
wchar_t *__DIE_versionW(void);

#endif // SINGLEFILESCAN_H
