#ifndef SCRIPTTEXT_H
#define SCRIPTTEXT_H

#include "scriptgeneric.h"

#include "../textfile.h"
//#include <dialogoptions.h>

class scriptText : public scriptGeneric
{
    Q_OBJECT
public:
    explicit scriptText(QObject *parent = 0);
    void setData(Binary *binary);

signals:
//    void appendError(QString sString);

    
public slots:
//    void _log(QString sString);
//    unsigned int swapBytes(unsigned int nValue);
//    QString upperCase(QString sString);


    bool isPlainText();


    bool compare(QString sSignature,unsigned int nOffset=0);
//    bool compareHeader(QString sSignature);
    bool compareEP(QString sSignature,unsigned int nOffset=0);
//    bool compareEPn(QString sSignature,unsigned int nOffset);

    QString getString(unsigned int nOffset,unsigned int nSize=50);
    unsigned int readDword(unsigned int nOffset);
    unsigned short readWord(unsigned int nOffset);
    unsigned char readByte(unsigned int nOffset);
    unsigned int getSize();
    QString getFileDirectory();
    QString getFileBaseName();
    QString getFileSuffix();
    QString getFileCompleteSuffix();
    int findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue);
    int findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue);
    int findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue);
    int findString(unsigned int nOffset, unsigned int nSize, QString sValue);
    int findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature);
    bool isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature);

    float calculateEntropy(unsigned int nOffset, unsigned int nSize);
    QString getSignature(unsigned int nOffset,unsigned int nSize);
    QString calculateMD5(unsigned int nOffset, unsigned int nSize);
private:
    Binary *binary;
    
};

#endif // SCRIPTTEXT_H
