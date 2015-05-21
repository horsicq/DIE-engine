#ifndef SCRIPTGENERIC_H
#define SCRIPTGENERIC_H

#include <QObject>
#include <QByteArray>
#include <QTime>

class scriptGeneric : public QObject
{
    Q_OBJECT
public:
    explicit scriptGeneric(QObject *parent = 0);

signals:
    void appendError(QString sString);


public slots:
    void _log(QString sString);
    unsigned int swapBytes(unsigned int nValue);
    QString upperCase(QString sString);

    virtual bool compare(QString sSignature,unsigned int nOffset)=0;
    //    virtual bool compareReg(QString sSignature,unsigned int nOffset)=0;
    //    virtual bool compareHeader(QString sSignature)=0;
    virtual bool compareEP(QString sSignature,unsigned int nOffset)=0;
    //    virtual bool compareEP(unsigned int nOffset,QString sSignature)=0;

    virtual QString getString(unsigned int nOffset,unsigned int nSize=50)=0;
    virtual unsigned int readDword(unsigned int nOffset)=0;
    virtual unsigned short readWord(unsigned int nOffset)=0;
    virtual unsigned char readByte(unsigned int nOffset)=0;
    virtual unsigned int getSize()=0;
    virtual QString getFileDirectory()=0;
    virtual QString getFileBaseName()=0; // new
    virtual QString getFileSuffix()=0; // new
    virtual QString getFileCompleteSuffix()=0; // new
    virtual int findByte(unsigned int nOffset, unsigned int nSize, unsigned char cValue)=0;
    virtual int findWord(unsigned int nOffset, unsigned int nSize, unsigned short sValue)=0;
    virtual int findDword(unsigned int nOffset, unsigned int nSize, unsigned int nValue)=0;
    virtual int findString(unsigned int nOffset, unsigned int nSize, QString sValue)=0;
    virtual int findSignature(unsigned int nOffset, unsigned int nSize, QString sSignature)=0;
    virtual bool isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature)=0;
    virtual float calculateEntropy(unsigned int nOffset, unsigned int nSize)=0;
    virtual QString getSignature(unsigned int nOffset,unsigned int nSize)=0;

    virtual QString calculateMD5(unsigned int nOffset, unsigned int nSize)=0;

public slots:

};

#endif // SCRIPTGENERIC_H
