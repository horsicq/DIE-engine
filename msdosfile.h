// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#ifndef MSDOSFILE_H
#define MSDOSFILE_H

#include "binary.h"

class MSDOSFile : public Binary
{
    Q_OBJECT

public:
    explicit MSDOSFile(QObject *parent=nullptr);
    void entryPointLoad();

public slots:
    bool compareEPfast(QString sSignature);
    bool isValid();

    unsigned int getDosHeaderOffset();
    unsigned int getDosHeaderSize();

    unsigned short getDosHeader_magic();
    unsigned short getDosHeader_cblp();
    unsigned short getDosHeader_cp();
    unsigned short getDosHeader_crlc();
    unsigned short getDosHeader_cparhdr();
    unsigned short getDosHeader_minalloc();
    unsigned short getDosHeader_maxalloc();
    unsigned short getDosHeader_ss();
    unsigned short getDosHeader_sp();
    unsigned short getDosHeader_csum();
    unsigned short getDosHeader_ip();
    unsigned short getDosHeader_cs();
    unsigned short getDosHeader_lfarlc();
    unsigned short getDosHeader_ovno();
    unsigned short getDosHeader_res(int nIndex);
    unsigned short getDosHeader_oemid();
    unsigned short getDosHeader_oeminfo();
    unsigned short getDosHeader_res2(int nIndex);

    void setDosHeader_magic(unsigned short sValue);
    void setDosHeader_cblp(unsigned short sValue);
    void setDosHeader_cp(unsigned short sValue);
    void setDosHeader_crlc(unsigned short sValue);
    void setDosHeader_cparhdr(unsigned short sValue);
    void setDosHeader_minalloc(unsigned short sValue);
    void setDosHeader_maxalloc(unsigned short sValue);
    void setDosHeader_ss(unsigned short sValue);
    void setDosHeader_sp(unsigned short sValue);
    void setDosHeader_csum(unsigned short sValue);
    void setDosHeader_ip(unsigned short sValue);
    void setDosHeader_cs(unsigned short sValue);
    void setDosHeader_lfarlc(unsigned short sValue);
    void setDosHeader_ovno(unsigned short sValue);
    void setDosHeader_res(short sValue,int nIndex);
    void setDosHeader_oemid(unsigned short sValue);
    void setDosHeader_oeminfo(unsigned short sValue);
    void setDosHeader_res2(short sValue,int nIndex);

    unsigned int getDosHeader_lfanew();
    void setDosHeader_lfanew(unsigned int nValue);

    virtual unsigned long long OffsetToRVA(unsigned long long nOffset);
    virtual unsigned long long RVAToOffset(unsigned long long nRVA);
    virtual unsigned long long VAToOffset(unsigned long long nVA);
    virtual unsigned long long VAToOffset32(unsigned long long nVA);
    virtual unsigned long long OffsetToVA(unsigned long long nOffset);

    unsigned int getOverlayOffset();
    unsigned int getOverlaySize();
    bool isOverlayPresent();
    bool compareOverlay(QString sSignature,unsigned int nOffset=0);

    unsigned int getEntryPointOffset();

    unsigned int getBaseOffset();

    bool isNE();
    bool isLE();
    bool isLX();

    bool compareEP(QString sSignature,unsigned int nOffset=0);
};

#endif // MSDOSFILE_H
