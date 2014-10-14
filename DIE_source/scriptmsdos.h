#ifndef SCRIPTMSDOS_H
#define SCRIPTMSDOS_H

#include "scriptgeneric.h"
#include "../msdosfile.h"

class scriptMSDOS : public scriptGeneric
{
    Q_OBJECT
public:
    explicit scriptMSDOS(QObject *parent = 0);
    void setData(MSDOSFile *msdosfile);

    
public slots:

    bool compareEP(QString sSignature,unsigned int nOffset=0);
//    bool compareEPn(QString sSignature,unsigned int nOffset);

    QString getString(unsigned int nOffset,unsigned int nSize=50);
    unsigned int readDword(unsigned int nOffset);
    unsigned short readWord(unsigned int nOffset);
    unsigned char readByte(unsigned int nOffset);
    unsigned int getSize();
//    bool compareHeader(QString sSignature);
    bool compare(QString sSignature,unsigned int nOffset=0);
    QString getFileDirectory();
    QString getFileBaseName();
    QString getFileSuffix();
    QString getFileCompleteSuffix();
    int findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue);
    int findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue);
    int findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue);
    int findString(unsigned int nOffset, unsigned int nSize, QString sValue);
    int findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature);

    bool isNE();
    bool isLE();
    bool isLX();

    bool isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature);

    float calculateEntropy(unsigned int nOffset, unsigned int nSize);
    QString getSignature(unsigned int nOffset,unsigned int nSize);
    QString calculateMD5(unsigned int nOffset, unsigned int nSize);
    unsigned int getOverlayOffset();
    unsigned int getOverlaySize();
    bool isOverlayPresent();
    bool compareOverlay(QString sSignature,unsigned int nOffset=0);

    int getEntryPointOffset();
private:
    MSDOSFile *msdosfile;
};

#endif // SCRIPTMSDOS_H
