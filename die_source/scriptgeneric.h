// Copyright (c) 2012-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
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
