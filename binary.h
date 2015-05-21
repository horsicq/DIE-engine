#ifndef BINARY_H
#define BINARY_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QList>
#include <QByteArray>
#include "_macros.h"
#if  defined(_WIN32)

#include <windows.h>
#else
#include "__windows.h"
#endif
//#if  defined(_WIN32)
#include "__unix.h"
//#endif

#include "__mac.h"

#include <math.h>
#include <QRegExp>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDir>
#include <QFileInfo>
#include <QTime>
#include <QCryptographicHash>

//#define BEA_ENGINE_STATIC
//#define BEA_USE_STDCALL
//#include "../../../cpp/_lib/beaengine-win32/headers/BeaEngine.h"

struct __MEMSTRUCT
{
    bool bIsChanged;
    unsigned int nOffset;
    //    unsigned int nSize;
    QByteArray baData;
};

class Binary : public QObject
{
#define EPCODE 0x400
#define BUFFER_SIZE 0x1000
#define MEMORYCOUNT 10
    Q_OBJECT
public:

    explicit Binary(QObject *parent = 0);
    ~Binary();

    unsigned char readByteFromFile(unsigned int nOffset);
    unsigned short readWordFromFile(unsigned int nOffset);
    unsigned int readDwordFromFile(unsigned int nOffset);
    unsigned long long readQwordFromFile(unsigned int nOffset);

    //    unsigned char readByteFromMemory(unsigned int nOffset);
    //    unsigned short readWordFromMemory(unsigned int nOffset);
    //    unsigned int readDwordFromMemory(unsigned int nOffset);
    //    unsigned long long readQwordFromMemory(unsigned int nOffset);

    void writeByteToFile(unsigned int nOffset,unsigned char cValue);
    void writeWordToFile(unsigned int nOffset,unsigned short sValue);
    void writeDwordToFile(unsigned int nOffset,unsigned int nValue);
    void writeQwordToFile(unsigned int nOffset,unsigned long long qValue);

    //    void writeByteToMemory(unsigned int nOffset,unsigned char cValue);
    //    void writeWordToMemory(unsigned int nOffset,unsigned short sValue);
    //    void writeDwordToMemory(unsigned int nOffset,unsigned int nValue);
    //    void writeQwordToMemory(unsigned int nOffset,unsigned long long qValue);
    //    bool isArrayInMemory(unsigned int nOffset,unsigned int nSize);
    bool readArrayFromFile(unsigned int nOffset,char *pBuffer,unsigned int nSize);
    bool writeArrayToFile(unsigned int nOffset,char *pBuffer,unsigned int nSize);
    bool readArray(unsigned int nOffset,char *pBuffer,unsigned int nSize);
    QByteArray readArray(unsigned int nOffset,unsigned int nSize);
    bool writeArray(unsigned int nOffset,char *pBuffer,unsigned int nSize);
    //    bool readArrayFromMemory(unsigned int nOffset,char *pBuffer,unsigned int nSize);
    //    bool writeArrayToMemory(unsigned int nOffset,char *pBuffer,unsigned int nSize);

    QByteArray readArrayFromEntryPoint(unsigned int nSize);

    //    void updateFile();

    void remap();

    bool createFile(QString sFileName,unsigned int nSize);
    bool deleteFile();
    static bool copy(Binary *pFileDest,unsigned int nOffsetDest,Binary *pFileSource,unsigned int nOffsetSource,unsigned int nSize);
    bool copy(Binary *pFileSource);

    //    void memoryReload();
    //    bool addMemory(unsigned int nOffset,unsigned int nSize=0x1000);
    //    void clearMemory();
    void close();
    static bool dataToHex(char *pData,unsigned int nDataSize,char *pOut);

    static bool signatureCompare(char *pOffset, char *pSignature, int nSize);
    static bool memoryCompare(char *pMemory1, char *pMemory2, int nSize);
    static unsigned int findDataInMemory(char *pMemory,unsigned int nMemorySize,char *pData,unsigned int nDataSize);
    static unsigned int findSignatureInMemory(char *pMemory,unsigned int nMemorySize,QString sSignature);
    //    QByteArray getHeaderData();


    bool reopen();
    bool isReadOnly();
    bool resize(unsigned long long nNewSize);

    virtual unsigned long long OffsetToRVA(unsigned long long nOffset);
    virtual unsigned long long RVAToOffset(unsigned long long nRVA);
    virtual unsigned long long VAToOffset(unsigned long long nVA);
    virtual unsigned long long VAToOffset32(unsigned long long nVA);
    virtual unsigned long long OffsetToVA(unsigned long long nOffset);

    QString getSignature(unsigned int nOffset,unsigned int nSize);

signals:
    void appendError(QString sError);
    void appendWarning(QString sError);
    void isReadOnly(bool bState);

private slots:

    //    bool compare(char *pszSignature,unsigned int nOffset=0);
    //    bool compareHeader(char *pszSignature);
    //    bool compareEPfast(char *pszSignature);

public slots:
    bool setFileName(QString sFileName,bool bReadHeader=true);
    QString getFileName();
    unsigned long long size();
    QString getFileBaseName(); // new
    QString getFileSuffix(); // new
    QString getFileCompleteSuffix(); // new
    QString getFileDirectory();

    unsigned char readByte(unsigned int nOffset);
    unsigned short readWord(unsigned int nOffset,bool bReverse=false);
    unsigned int readDword(unsigned int nOffset,bool bReverse=false);
    unsigned long long readQword(unsigned int nOffset,bool bReverse=false);
    void writeByte(unsigned int nOffset,unsigned char cValue);
    void writeWord(unsigned int nOffset,unsigned short sValue,bool bReverse=false);
    void writeDword(unsigned int nOffset,unsigned int nValue,bool bReverse=false);
    void writeQword(unsigned int nOffset,unsigned long long qValue,bool bReverse=false);


    unsigned short swapWord(unsigned short sValue);
    unsigned int swapDword(unsigned int nValue);
    unsigned int swapQword(unsigned long long qValue);

    bool isPlainText();

    QString getString(unsigned int nOffset,unsigned int nMaxSize=100);
    QString getAnsiString(unsigned int nOffset,unsigned int nMaxSize=100);
    void setString(unsigned int nOffset,QString sValue);

    bool compare(QString sSignature,unsigned int nOffset=0);
    bool compareEPfast(QString sSignature,int nOffset);
    bool compareEP(QString sSignature,unsigned int nOffset);
    bool _compare(QString sSignature,unsigned int nOffset);
    //    bool compareFast(QString sSignature,unsigned int nOffset);
    //    bool compareHeader(QString sSignature);

    static QString convertSignature(QString sSignature);

    float calculateEntropy(unsigned int nOffset=0,unsigned int nDataSize=-1);
    static float calculateEntropy(QString sFileName,unsigned int nOffset=0,unsigned int nDataSize=-1);
    QString calculateMD5(unsigned int nOffset=0,unsigned int nDataSize=-1);
    static QString calculateMD5(QString sFileName,unsigned int nOffset=0,unsigned int nDataSize=-1);
    bool isOffsetValid(unsigned int nOffset);
    bool isSizeValid(unsigned int nSize);
    bool isOffsetAndSizeValid(unsigned int nOffset,unsigned int nSize);

    QList<QString> getTypes();
    QString getType();
    QString getDisasmMode();
    bool dump(QString sFileName,unsigned int nOffset,unsigned int nSize);
    static QString getType(QString sFileName);
    static unsigned long long getSize(QString sFileName);

    void _loadEntryPoint();

    void entryPointLoad();

    unsigned int findByte(unsigned int nOffset,unsigned int nSize,unsigned char cValue);
    unsigned int findWord(unsigned int nOffset,unsigned int nSize,unsigned short sValue);
    unsigned int findDword(unsigned int nOffset,unsigned int nSize,unsigned int nValue);
    unsigned int findArray(unsigned int nOffset,unsigned int nSize,char *pArray,unsigned int nArraySize);
    unsigned int findString(unsigned int nOffset,unsigned int nSize,QString sString);
    unsigned int findSignature(unsigned int nOffset,unsigned int nSize,QString sSignature);

    bool isSignaturePresent(unsigned int nOffset,unsigned int nSize,QString sSignature);

    //    unsigned int intelDisasm(DISASM *pDisasm,unsigned int nOffset,unsigned int nSize);
    //    QByteArray intelDisasmByteArray(unsigned int nMode,unsigned int nOffset,unsigned int nSize);
    //    QString intelDisasmString(unsigned int nMode,unsigned int nOffset,unsigned int nSize);
protected:
    QFile file;
    //    QByteArray baHeader;
    //    QList<__MEMSTRUCT> memorylist;
    //    unsigned int nMemoryPosition;
    //    __MEMSTRUCT _memorylist[MEMORYCOUNT];
    bool bIsReadOnly;
    QString sFileName;
    bool bReadHeader;
    bool bIsEntryPontLoad;
    unsigned int nEntryPointOffset;
    QByteArray baEntryPoint;
    //    char pEntryPoint[EPCODE];
    //    unsigned int nEntryPointSize;
    unsigned long long ___nSize;

    unsigned char *___pMemory;

    int nIsReverse;

private:
    //    void _clearMemory();
    //    void addMemory(unsigned int nOffset,unsigned int nSize);


};

#endif // BINARY_H
