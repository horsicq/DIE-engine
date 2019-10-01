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
#ifndef SCRIPTMACH_H
#define SCRIPTMACH_H

#include "scriptgeneric.h"
#include "../machfile.h"

class scriptMACH : public scriptGeneric
{
    Q_OBJECT
public:
    explicit scriptMACH(QObject *parent=nullptr);
    void setData(MACHFile *machfile);

public slots:
    bool compareEP(QString sSignature,unsigned int nOffset=0);
    //    bool compareEPn(QString sSignature,unsigned int nOffset);
    QString getString(unsigned int nOffset,unsigned int nSize=50);
    unsigned long long readQword(unsigned int nOffset);
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
    bool isSectionNamePresent(QString sSectionName);
    bool isSignaturePresent(unsigned int nOffset, unsigned int nSize, QString sSignature);
    bool compareReg(QString sSignature,unsigned int nOffset);
    bool isLibraryPresent(QString sLibrary);
    unsigned int getLibraryCurrentVersion(QString sLibrary);
    int getSectionNumber(QString sSectionName);
    QString getGeneralOptions();
    int getNumberOfSections();
    int getNumberOfSegments();
    double calculateEntropy(unsigned int nOffset, unsigned int nSize);
    QString getSignature(unsigned int nOffset,unsigned int nSize);
    QString calculateMD5(unsigned int nOffset, unsigned int nSize);
    unsigned int getSectionFileOffset(int nSection);
    unsigned int getSectionFileSize(int nSection);
    QString getSectionName(int nSection);
    QString getSegmentName(int nSegment);
    unsigned int getSegmentFileSize(int nSegment);
    unsigned int getSegmentFileOffset(int nSegment);
    long long RVAToOffset(unsigned long long nRVA);
    long long VAToOffset(unsigned long long nVA);
    long long OffsetToVA(unsigned long long nOffset);
    long long OffsetToRVA(unsigned long long nOffset);
    unsigned int getOverlaySize();
    unsigned int getOverlayOffset();
    bool isOverlayPresent();
    bool compareOverlay(QString sSignature,unsigned int nOffset=0);
    long long getEntryPointOffset();

private:
    MACHFile *machfile;
};

#endif // SCRIPTMACH_H
